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
#define BIT_PUMP_START 						0x00000040
#define BIT_PUMP_STOP 						0x00000080

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
*                              				Private function prototypes
*********************************************************************************************************
*/
static void bsp_InitInport(struct tagIO_OPERATE_TypeDef *this);
static uint32_t bsp_InportRead(struct tagIO_OPERATE_TypeDef *this);
static void bsp_InportCycleTask(struct tagIO_OPERATE_TypeDef *this);

static void bsp_InitOutport(struct tagIO_OPERATE_TypeDef *this);
static void bsp_SetOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);
static void bsp_ClrOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);
static void bsp_RevOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);
static void bsp_SetOutport(struct tagIO_OPERATE_TypeDef *this, uint32_t value);
static uint32_t bsp_GetOutport(struct tagIO_OPERATE_TypeDef *this);
static void bsp_OutportCycleTask(struct tagIO_OPERATE_TypeDef *this);

static void bsp_InitLamp(struct tagIO_OPERATE_TypeDef *this);
static void bsp_LampCycleTask(struct tagIO_OPERATE_TypeDef *this);


/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/
static IO_OPERATE_TypeDef g_IOInput = {
	.init						= bsp_InitInport,
	.read						= bsp_InportRead,
	.readSync					= bsp_InportCycleTask,
};

static IO_OPERATE_TypeDef g_IOOutput = {
	.init						= bsp_InitOutport,
	.read						= bsp_GetOutport,
	
	.setOutputBit				= bsp_SetOutportBit,
	.clrOutputBit				= bsp_ClrOutportBit,
	.revOutputBit				= bsp_RevOutportBit,
	.setOutput					= bsp_SetOutport,
	.writeSync					= bsp_OutportCycleTask,
};

static IO_OPERATE_TypeDef g_IOLamp = {
	.init						= bsp_InitLamp,
	.read						= bsp_GetOutport,
	
	.setOutputBit				= bsp_SetOutportBit,
	.clrOutputBit				= bsp_ClrOutportBit,
	.revOutputBit				= bsp_RevOutportBit,
	.setOutput					= bsp_SetOutport,
	.writeSync					= bsp_LampCycleTask,
};

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
* Function Name : GetIO_InputHandle
* Description	: ��ȡIO������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
IO_OPERATE_TypeDef *GetIO_InputHandle(void)
{
	return &g_IOInput;
}	

/*
*********************************************************************************************************
* Function Name : GetIO_OutputHandle
* Description	: ��ȡIO������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
IO_OPERATE_TypeDef *GetIO_OutputHandle(void)
{
	return &g_IOOutput;
}

/*
*********************************************************************************************************
* Function Name : GetIO_LampHandle
* Description	: ��ȡָʾ�ƾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
IO_OPERATE_TypeDef *GetIO_LampHandle(void)
{
	return &g_IOLamp;
}

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
static void bsp_InitInport(struct tagIO_OPERATE_TypeDef *this)
{
	Init_74HC165();	
	
	const uint32_t IO_SHAKE_TIME = 20;
	bsp_StartAutoTimer(ID_TIM_BSP_IO, IO_SHAKE_TIME);
	
	this->shadowReg = 0x00000000 ^ INPUT_REV;
	this->dataReg   = 0x00000000 ^ INPUT_REV;
	if (this->readSync)
	{
		this->readSync(this);
	}
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
static uint32_t bsp_InportRead(struct tagIO_OPERATE_TypeDef *this)
{	
	return (this->dataReg ^ INPUT_REV);
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
static void bsp_InportCycleTask(struct tagIO_OPERATE_TypeDef *this)
{
	/* ���� */
	if ( !bsp_CheckTimer(ID_TIM_BSP_IO) )
	{
		return;
	}
	
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
static void bsp_SetOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit)
{ 
	SET_BIT(this->shadowReg, bit);
} 

/*
*********************************************************************************************************
* Function Name : bsp_ClrOutportBit
* Description	: ������λ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_ClrOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit)
{ 
	CLEAR_BIT(this->shadowReg, bit);
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
static void bsp_RevOutportBit(struct tagIO_OPERATE_TypeDef *this, uint32_t bit)
{ 
	XOR_BIT(this->shadowReg, bit); 
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
static void bsp_SetOutport(struct tagIO_OPERATE_TypeDef *this, uint32_t value)
{ 
	WRITE_REG(this->shadowReg, value);
	
	this->dataReg = ~this->shadowReg;
	
	/**
		��Ҫ����ִ��ͬ������Ϊ��ʱ���ݼĴ�����ֵΪ��ʱֵ,
		�����ʱ��ȡ���ݼĴ��������ִ���
	*/
	if (this->writeSync)
	{
		this->writeSync(this);
	}
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
static uint32_t bsp_GetOutport(struct tagIO_OPERATE_TypeDef *this)
{
	return this->dataReg;
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
static void bsp_OutportCycleTask(struct tagIO_OPERATE_TypeDef *this)
{
	uint32_t flag = this->dataReg ^ this->shadowReg; 
	
	//����������Ҫ�ı䣬ˢ�����
	if (flag)		
	{	
		if (flag & BSP_DO_BEEP)
		{
			if (this->shadowReg & BSP_DO_BEEP)
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
			}
		}
		
		this->dataReg = this->shadowReg;
	}
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
static void bsp_InitOutport(struct tagIO_OPERATE_TypeDef *this)
{
	bsp_Init_74HC595();
	bsp_InitBeep();
	
	this->shadowReg = 0x00000000;
	this->dataReg = ~this->shadowReg;
	if (this->writeSync)
	{
		this->writeSync(this);
	}
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
static void bsp_InitLamp(struct tagIO_OPERATE_TypeDef *this)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	LED_GPIO_CLK_ENABLE(); 
	
	GPIO_InitStructure.Pin = LED_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
	HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
	
	this->shadowReg = 0x00000000;
	this->dataReg = ~this->shadowReg;
	if (this->writeSync)
	{
		this->writeSync(this);
	}
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
static void bsp_LampCycleTask(struct tagIO_OPERATE_TypeDef *this)
{
	uint32_t flag = this->dataReg ^ this->shadowReg; 
	
	if( flag )		//����������Ҫ�ı䣬ˢ�����
	{	
		if (flag & BSP_LAMP_BOARD_COM)
		{
			if (this->shadowReg & BSP_LAMP_BOARD_COM)
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
			
			if (flag & BSP_LAMP_PUMP_START)
			{
				if (this->shadowReg & BSP_LAMP_PUMP_START)
				{
					SET_BIT(value, BIT_PUMP_START);
				}
			}
			
			if (flag & BSP_LAMP_PUMP_STOP)
			{
				if (this->shadowReg & BSP_LAMP_PUMP_STOP)
				{
					SET_BIT(value, BIT_PUMP_STOP);
				}
			}
			
			bsp_74HC595_Write(value);
		}

		this->dataReg = this->shadowReg;
	}
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
	g_IOInput.init(&g_IOInput);
	
	/* ���IO��ʼ�� */
	g_IOOutput.init(&g_IOOutput);
	
	/* С�Ƴ�ʼ�� */
	g_IOLamp.init(&g_IOLamp);
	
	ECHO(DEBUG_BSP_INIT, "IO��ʼ�� .......... OK");
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
