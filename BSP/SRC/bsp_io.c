/*
*********************************************************************************************************
* @file    	bsp_io.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-13 19:53:06
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	IO����Դ�ļ�
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
#include "bsp.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				74HC165
*********************************************************************************************************
*/
#define	HC165_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOE_CLK_ENABLE()

#define	HC165_CLK_GPIO_PIN					GPIO_PIN_0
#define	HC165_CLK_GPIO_PORT					GPIOE

#define	HC165_LOCK_GPIO_PIN					GPIO_PIN_1
#define	HC165_LOCK_GPIO_PORT				GPIOE

#define	HC165_DAT_GPIO_PIN					GPIO_PIN_2
#define	HC165_DAT_GPIO_PORT					GPIOE
	
#define	HC165_GPIO_CLK_SET()				HAL_GPIO_WritePin(HC165_CLK_GPIO_PORT,HC165_CLK_GPIO_PIN,GPIO_PIN_SET)
#define	HC165_GPIO_LOCK_SET() 				HAL_GPIO_WritePin(HC165_LOCK_GPIO_PORT,HC165_LOCK_GPIO_PIN,GPIO_PIN_SET)
	
#define	HC165_GPIO_CLK_CLEAR()				HAL_GPIO_WritePin(HC165_CLK_GPIO_PORT,HC165_CLK_GPIO_PIN,GPIO_PIN_RESET)	
#define	HC165_GPIO_LOCK_CLEAR() 			HAL_GPIO_WritePin(HC165_LOCK_GPIO_PORT,HC165_LOCK_GPIO_PIN,GPIO_PIN_RESET)

#define	HC165_GPIO_DAT_READ()   			HAL_GPIO_ReadPin(HC165_DAT_GPIO_PORT,HC165_DAT_GPIO_PIN)

/*
*********************************************************************************************************
*                              				74HC595
*********************************************************************************************************
*/
#define USE_HC595_EN_GPIO					0x00U	//ʹ��оƬʹ����������

#define	HC595_GPIO_CLK_ENABLE()				do {\
													__HAL_RCC_GPIOB_CLK_ENABLE();\
													__HAL_RCC_GPIOE_CLK_ENABLE();\
											} while (0)

#define	HC595_RCK_GPIO_PORT					GPIOE
#define	HC595_RCK_GPIO_PIN					GPIO_PIN_4

#define	HC595_SCK_GPIO_PORT					GPIOE
#define	HC595_SCK_GPIO_PIN					GPIO_PIN_0

#define	HC595_SDA_GPIO_PORT					GPIOE
#define	HC595_SDA_GPIO_PIN					GPIO_PIN_3

#define	HC595_SE_GPIO_PORT					GPIOB
#define	HC595_SE_GPIO_PIN					GPIO_PIN_9
	
#define	HC595_GPIO_RCK_SET()				HAL_GPIO_WritePin(HC595_RCK_GPIO_PORT,HC595_RCK_GPIO_PIN,GPIO_PIN_SET)
#define	HC595_GPIO_SCK_SET() 				HAL_GPIO_WritePin(HC595_SCK_GPIO_PORT,HC595_SCK_GPIO_PIN,GPIO_PIN_SET)
#define	HC595_GPIO_SDA_SET() 				HAL_GPIO_WritePin(HC595_SDA_GPIO_PORT,HC595_SDA_GPIO_PIN,GPIO_PIN_SET)	
#define	HC595_GPIO_SE_SET() 				HAL_GPIO_WritePin(HC595_SE_GPIO_PORT,HC595_SE_GPIO_PIN,GPIO_PIN_SET)
 
#define	HC595_GPIO_RCK_CLEAR()				HAL_GPIO_WritePin(HC595_RCK_GPIO_PORT,HC595_RCK_GPIO_PIN,GPIO_PIN_RESET)
#define	HC595_GPIO_SCK_CLEAR() 				HAL_GPIO_WritePin(HC595_SCK_GPIO_PORT,HC595_SCK_GPIO_PIN,GPIO_PIN_RESET)
#define	HC595_GPIO_SDA_CLEAR() 				HAL_GPIO_WritePin(HC595_SDA_GPIO_PORT,HC595_SDA_GPIO_PIN,GPIO_PIN_RESET)	
#define	HC595_GPIO_SE_CLEAR() 				HAL_GPIO_WritePin(HC595_SE_GPIO_PORT,HC595_SE_GPIO_PIN,GPIO_PIN_RESET)

/*
*********************************************************************************************************
*                              				������
*********************************************************************************************************
*/
#define	BEEP_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define	BEEP_GPIO_PORT  					GPIOB
#define	BEEP_GPIO_PIN						GPIO_PIN_8

/*
*********************************************************************************************************
*                              				С��
*********************************************************************************************************
*/
#define	LED_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define	LED_GPIO_PORT 						GPIOB
#define	LED_GPIO_PIN						GPIO_PIN_5

#define INPUT_REV							(0)

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef enum
{
	BIT_PUMP_START 	= 0x00000040,
	BIT_PUMP_STOP 	= 0x00000080,
}LAMP_74HC595_ENUM;

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
static uint32_t g_InportData;
static uint32_t g_InportShadow;
static uint32_t g_OutportData;
static uint32_t g_OutportShadow;
static uint32_t g_LampData;
static uint32_t g_LampShadow;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/  

/*
*********************************************************************************************************
*                              				����˿�
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : Init_74HC165
* Description	: ��ʼ��оƬ74HC165
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Init_74HC165( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	HC165_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = HC165_CLK_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(HC165_CLK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC165_LOCK_GPIO_PIN;
	HAL_GPIO_Init(HC165_LOCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC165_DAT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(HC165_DAT_GPIO_PORT, &GPIO_InitStructure);
	
	HC165_GPIO_CLK_CLEAR();
	HC165_GPIO_LOCK_SET();
}

/*
*********************************************************************************************************
* Function Name : __bsp_74HC165_Read32
* Description	: ��74HC165��ȡ32λ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static uint32_t __bsp_74HC165_Read32( void )
{
	uint8_t i;
	uint32_t value = 0;
	
	HC165_GPIO_LOCK_CLEAR();
	bsp_DelayUS(1);
	HC165_GPIO_LOCK_SET(); 
	
	for (i=0; i<32; i++)
	{		  			
		value <<= 1;
		if( HC165_GPIO_DAT_READ() )
		{
			value |= 0x01;
		} 
		
		HC165_GPIO_CLK_SET();
		bsp_DelayUS(1);
		HC165_GPIO_CLK_CLEAR();	
	}
	
	return value;
}

/*
*********************************************************************************************************
* Function Name : bsp_74HC165_Read
* Description	: ��74HC165��ȡ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_74HC165_Read( void )
{
	return ( ~__bsp_74HC165_Read32() );
}

/*
*********************************************************************************************************
* Function Name : InportInit
* Description	: ��ʼ������˿�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitInport(void)
{
	Init_74HC165();	
	
	g_InportShadow = 0x00000000 ^ INPUT_REV;
	g_InportData   = 0x00000000 ^ INPUT_REV;
	
	bsp_InportCycleTask();	
}

/*
*********************************************************************************************************
* Function Name : bsp_InportRead
* Description	: ��ȡ����˿�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_InportRead( void )
{	
	return (g_InportData^INPUT_REV);
}

/*
*********************************************************************************************************
* Function Name : bsp_InportCycleTask
* Description	: �˿�����ѭ������(�ڲ���ʵ�ַ���)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InportCycleTask(void)
{
	uint32_t tempu32 = 0;
	
	/* 	
		�����⵽�����źŷ����仯�������ڽ�����Ӱ�ӼĴ������¸������ٴαȶ�
		�����ź��Ƿ����仯����������ˣ���Ӱ�ӼĴ�����ֵ�洢������Ĵ�����
		Ч���൱���ӳ�һ�����ڣ��𵽷���Ч����
	*/	
	if (tempu32 != g_InportShadow)
	{
		g_InportShadow = tempu32;
	}	
	else
	{
		g_InportData = g_InportShadow;
	}
	
	ECHO(DEBUG_BSP_IO,"in   : %08x H",g_InportData^INPUT_REV);
}

/*
*********************************************************************************************************
*                              				����˿�
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_Init_74HC595
* Description	: ��ʼ��оƬ74HC595
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_Init_74HC595( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	HC595_GPIO_CLK_ENABLE(); 
	
	GPIO_InitStructure.Pin = HC595_RCK_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(HC595_RCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC595_SCK_GPIO_PIN;	
	HAL_GPIO_Init(HC595_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = HC595_SDA_GPIO_PIN;	
	HAL_GPIO_Init(HC595_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	#if (USE_HC595_EN_GPIO == 0x01U)
		GPIO_InitStructure.Pin = HC595_SE_GPIO_PIN;
		HAL_GPIO_Init(HC595_SE_GPIO_PORT, &GPIO_InitStructure);
	#endif
	
 	HC595_GPIO_RCK_CLEAR();
 	HC595_GPIO_SCK_CLEAR();
 	HC595_GPIO_SDA_CLEAR();
	
	#if (USE_HC595_EN_GPIO == 0x01U)
		HC595_GPIO_SE_CLEAR();
	#endif
}

/*
*********************************************************************************************************
* Function Name : __bsp_74HC595_Write32
* Description	: ��74HC595д��32λ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __bsp_74HC595_Write32( uint32_t value )
{
	uint8_t i;
	
	for (i=0; i<32; i++)
	{		  	
		if (value & 0x80000000)
		{
			HC595_GPIO_SDA_SET();
		}
		else
		{
			HC595_GPIO_SDA_CLEAR();
		}
		
		HC595_GPIO_SCK_SET();
		bsp_DelayUS(1);
		HC595_GPIO_SCK_CLEAR();
		
		value<<=1;		
	}
	
	HC595_GPIO_RCK_SET();
	bsp_DelayUS(1);
	HC595_GPIO_RCK_CLEAR();
}

/*
*********************************************************************************************************
* Function Name : bsp_74HC595_Write
* Description	: ��74HC595д������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_74HC595_Write( uint32_t value )
{
	__bsp_74HC595_Write32(value);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitBeep
* Description	: ��������ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitBeep( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	BEEP_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = BEEP_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
}

/*
*********************************************************************************************************
* Function Name : bsp_SetOutportBit
* Description	: �������λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetOutportBit( uint32_t bit )
{ 
	SET_BIT(g_OutportShadow, bit);
} 

/*
*********************************************************************************************************
* Function Name : bsp_ClearOutportBit
* Description	: ������λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ClearOutportBit( uint32_t bit )
{ 
	CLEAR_BIT(g_OutportShadow, bit);
} 
  
/*
*********************************************************************************************************
* Function Name : bsp_ToggleOutportBit
* Description	: ��ת���λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ToggleOutportBit( uint32_t bit )
{ 
	XOR_BIT(g_OutportShadow, bit); 
} 
    
/*
*********************************************************************************************************
* Function Name : bsp_SetOutport
* Description	: �������ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetOutport(uint32_t out)
{ 
	WRITE_REG(g_OutportShadow, out);
	g_OutportData = ~g_OutportShadow;
}
 
/*
*********************************************************************************************************
* Function Name : bsp_GetOutport
* Description	: ��ȡ���ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetOutport( void )
{
	/* ��ֹ����ֵ�󣬲�δִ��ѭ�������¶�ȡ��ֵ���� */
	bsp_OutportCycleTask();
	
	return g_OutportData;
}

/*
*********************************************************************************************************
* Function Name : outport_task
* Description	: �˿�ѭ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_OutportCycleTask( void )
{
	uint32_t flag = g_OutportData ^ g_OutportShadow; 
	
	if ( flag )		//����������Ҫ�ı䣬ˢ�����
	{	
		if (flag & DO_BEEP)
		{
			if (g_OutportShadow & DO_BEEP)
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
			}
		}
		
		g_OutportData = g_OutportShadow;
	}

	ECHO(DEBUG_BSP_IO,"out  : %08x H",g_OutportData);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitOutport
* Description	: ��ʼ�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitOutport( void )
{
	bsp_Init_74HC595();
	bsp_InitBeep();
	
	g_OutportShadow = 0x00000000;
	g_OutportData = ~g_OutportShadow;
	
	bsp_OutportCycleTask();
}

/*
*********************************************************************************************************
*                              				С��
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitLamp
* Description	: С�Ƴ�ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitLamp( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	LED_GPIO_CLK_ENABLE(); 
	
	GPIO_InitStructure.Pin = LED_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
	
	g_LampShadow = 0x00000000;
	g_LampData = ~g_LampShadow;
	
	bsp_LampCycleTask();
}

/*
*********************************************************************************************************
* Function Name : bsp_SetLampBit
* Description	: ����С�����λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetLampBit( uint32_t bit )
{ 
	SET_BIT(g_LampShadow, bit);
} 

/*
*********************************************************************************************************
* Function Name : bsp_ClearLampBit
* Description	: ���С�����λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ClearLampBit( uint32_t bit )
{ 
	CLEAR_BIT(g_LampShadow, bit);
} 
  
/*
*********************************************************************************************************
* Function Name : bsp_ToggleLampBit
* Description	: ��תС�����λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ToggleLampBit( uint32_t bit )
{ 
	XOR_BIT(g_LampShadow, bit); 
} 
    
/*
*********************************************************************************************************
* Function Name : bsp_SetLamp
* Description	: ����С�����ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_SetLamp(uint32_t out)
{ 
	WRITE_REG(g_LampShadow, out);
	g_LampData = ~g_LampShadow;
}
 
/*
*********************************************************************************************************
* Function Name : bsp_GetLamp
* Description	: ��ȡС�����ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetLamp( void )
{
	/* ��ֹ����ֵ�󣬲�δִ��ѭ�������¶�ȡ��ֵ���� */
	bsp_LampCycleTask();
	
	return g_LampData;
}

/*
*********************************************************************************************************
* Function Name : bsp_LampCycleTask
* Description	: С��ѭ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_LampCycleTask( void )
{
	uint32_t tempu32 = g_LampData ^ g_LampShadow; 
	
	if( tempu32 )		//����������Ҫ�ı䣬ˢ�����
	{	
		if (tempu32 & LAMP_BOARD_COM)
		{
			if (g_LampShadow & LAMP_BOARD_COM)
			{
				HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
			}
		}
		
		/* оƬ-74HC595 */
		{
			uint32_t value = 0;
			
			if (tempu32 & LAMP_PUMP_START)
			{
				if (g_LampShadow & LAMP_PUMP_START)
				{
					SET_BIT(value, BIT_PUMP_START);
				}
			}
			
			if (tempu32 & LAMP_PUMP_STOP)
			{
				if (g_LampShadow & LAMP_PUMP_STOP)
				{
					SET_BIT(value, BIT_PUMP_STOP);
				}
			}
			
			bsp_74HC595_Write(value);
		}

		g_LampData = g_LampShadow;
	}

	ECHO(DEBUG_BSP_IO,"out  : %08x H",g_LampData);
}

/*
*********************************************************************************************************
* Function Name : bsp_InitIO
* Description	: ��ʼ��IO
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitIO( void )
{
	/* ����IO��ʼ�� */ 
	bsp_InitInport();
	
	/* ���IO��ʼ�� */
	bsp_InitOutport();
	
	/* С�Ƴ�ʼ�� */
	bsp_InitLamp();
	
	ECHO(DEBUG_BSP_INIT, "IO��ʼ�� .......... OK");
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
