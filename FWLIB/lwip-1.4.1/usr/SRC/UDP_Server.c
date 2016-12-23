/*
*********************************************************************************************************
* @file    	UDP_Server.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-28 10:14:06
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	UDP������Դ�ļ�
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "kernel.h"
#include "ethernetif.h"
#include "SeqQueue.h"
#include "lwip/netif.h"
#include "UDP_Server.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define UDP_SERVER_PORT    					9999   /* define the UDP local connection port */


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/
static struct udp_pcb *g_UDP_ServerHandle;
extern uint8_t ETH_RxBuff[];
extern SEQUEUE_TypeDef g_ETH_RxQueue;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void UDP_ServerReceive_CallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : UDP_ServerInit
* Description	: UDP��������ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void UDP_ServerInit( void )
{
	g_UDP_ServerHandle = udp_new();
	if (g_UDP_ServerHandle)
	{
		/* Bind the upcb to the UDP_PORT port */
		/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
		err_t err = udp_bind(g_UDP_ServerHandle, IP_ADDR_ANY, UDP_SERVER_PORT);
		if(err == ERR_OK)
		{
			/* Set a receive callback for the upcb */
			udp_recv(g_UDP_ServerHandle, UDP_ServerReceive_CallBack, NULL);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerReceive_CallBack
* Description	: UDP�������������ݻص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UDP_ServerReceive_CallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,\
				const ip_addr_t *addr, u16_t port)
{
	/**
	 * 	ʵ�ʲ��ԣ�һ̨�ͻ������ӷ�����������IP�������˿��򱾷������������ݾ�ʧ�ܣ�
	 *	�Ӷ�ʵ�ְ��״����ӵĿͻ��ˡ�
	 */
	if ( (*(uint32_t *)&pcb->remote_ip == 0) && (pcb->remote_port == 0) )
	{
		ECHO(DEBUG_BSP_ETH,"<UDP> �ͻ����������ӷ�������");

		udp_connect(pcb, addr, port);
	}
	
	if (p)
	{	
		uint8_t packageBuff[ETH_MAX_POOL_SIZE];
		uint32_t rxLen = ETH_GetPackageBody(p, packageBuff);
		
		for (uint32_t i=0; i<rxLen; ++i)
		{
			if (PushSeqQueue(&g_ETH_RxQueue, &packageBuff[i], PushSeqQueueU8_CallBack)\
				== STATUS_DATA_STRUCT_FALSE)
			{
				ECHO(DEBUG_BSP_ETH,"<UDP> ���������");
				
				break;
			}
		}
		
		/* Free the p buffer */
		pbuf_free(p);
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerReadByte
* Description	: ��ȡUDP����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerReadByte( uint8_t *data )
{
	if (PopSeqQueue(&g_ETH_RxQueue, data, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerWriteBytes
* Description	: ʹ��UDP��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerWriteBytes( uint8_t *IP_Addr, uint16_t Port, \
		const uint8_t *data, uint32_t lenth )
{
	err_t ret = ERR_OK;
	
	if (lenth)
	{
		/*
			PBUF_RAM����Ӧ�ó����Ͷ�̬�ز���������ʱ,PBUF_RAM���͵�PbufsҲ��ʹ�á� 
			�����������,pbufϵͳ����ΪӦ�����ݷ���洢�ռ�,ҲΪ��Ҫ���͵����ݵı�ͷ׼���ռ䡣
			pbufϵͳ����Ԥ��֪��Ϊ��Ҫ���͵�����׼��ʲô���ı�ͷ,���Ҽٶ���������
			��ͷ��С�ڱ���ʱ�ɶ�̬���á�
		*/
		struct pbuf *p = pbuf_alloc(PBUF_RAW, lenth, PBUF_RAM);
		if (p == NULL)
		{
			ECHO(DEBUG_BSP_ETH,"<UDP> �ڴ�������");
			
			return false;
		}
	
		/* copy data to pbuf */
		pbuf_take(p, data, lenth);
		ip_addr_t DestIPaddr;
		IP4_ADDR( &DestIPaddr, IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3] );
		ret = udp_sendto(g_UDP_ServerHandle, p, &DestIPaddr, Port);
		/* Free the p buffer */
		pbuf_free(p);
	}
	
	if (ret == ERR_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
*********************************************************************************************************
* Function Name : UDP_ServerEchoWriteBytes
* Description	: ʹ��UDP��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool UDP_ServerEchoWriteBytes( const uint8_t *data, uint32_t lenth )
{
	err_t ret = ERR_OK;
	
	if (lenth)
	{
		struct pbuf *p = pbuf_alloc(PBUF_RAW, lenth, PBUF_RAM);
		if (p == NULL)
		{
			ECHO(DEBUG_BSP_ETH,"<UDP> �ڴ�������");
			
			return false;
		}
	
		/* copy data to pbuf */
		pbuf_take(p, data, lenth);
		ret = udp_send(g_UDP_ServerHandle, p);
		/* Free the p buffer */
		pbuf_free(p);
	}
	
	if (ret == ERR_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
