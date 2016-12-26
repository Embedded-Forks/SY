/*
*********************************************************************************************************
* @file    	io.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-18 11:12:27
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	IO�ں�Դ�ļ�
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


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


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
static IO_TypeDef g_IO_Device;
static CYCLE_TASK_TypeDef g_BeepDevice;

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
* Function Name : IO_WriteOutputMask
* Description	: IOд�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_WriteOutputMask( IO_TypeDef *this, uint32_t clrMask, uint32_t setMask, uint32_t revMask )
{
	CLEAR_BIT(this->outputReg, clrMask);
	SET_BIT(this->outputReg, setMask);
	XOR_BIT(this->outputReg, revMask);
}

/*
*********************************************************************************************************
* Function Name : IO_WriteLampMask
* Description	: IOдָʾ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_WriteLampMask( IO_TypeDef *this, uint32_t clrMask, uint32_t setMask, uint32_t revMask )
{
	CLEAR_BIT(this->lampReg, clrMask);
	SET_BIT(this->lampReg, setMask);
	XOR_BIT(this->lampReg, revMask);
}

/*
*********************************************************************************************************
* Function Name : InitBeep
* Description	: ��ʼ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void InitBeep(CYCLE_TASK_TypeDef *this)
{	
	RepeatExecuteTaskInit(this, 10, NULL);
	RegisterRepeatExecuteTaskOnDevice_CallBack(this, __OpenBeep);
	RegisterRepeatExecuteTaskOffDevice_CallBack(this, __CloseBeep);
	
	ECHO(DEBUG_KERNEL_INIT, "��������ʼ�� .......... OK");
}

/*
*********************************************************************************************************
* Function Name : __OpenBeep
* Description	: �򿪷�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void __OpenBeep( void *devicePrt )
{
	/* �ӿ���Ӧ�ٶ� */
	#if 0
		IO_WriteOutputMask(devicePrt,0,IO_OUTPUT_MASK_BEEP,0);
	#else
		IO_OPERATE_TypeDef *outputHandle = GetIO_OutputHandle();
		
		outputHandle->setOutputBit(outputHandle, BSP_DO_BEEP);
		outputHandle->writeSync(outputHandle);
	#endif
	ECHO(DEBUG_KERNEL_IO, "[KERNEL-IO] �򿪷�����");
}

/*
*********************************************************************************************************
* Function Name : __CloseBeep
* Description	: �رշ�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void __CloseBeep( void *devicePrt )
{
	#if 0
		IO_WriteOutputMask(devicePrt,IO_OUTPUT_MASK_BEEP,0,0);
	#else
		IO_OPERATE_TypeDef *outputHandle = GetIO_OutputHandle();
		
		outputHandle->clrOutputBit(outputHandle, BSP_DO_BEEP);
		outputHandle->writeSync(outputHandle);
	#endif
	ECHO(DEBUG_KERNEL_IO, "[KERNEL-IO] �رշ�����");
}

/*
*********************************************************************************************************
* Function Name : GetBeepHandle
* Description	: ��ȡ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
CYCLE_TASK_TypeDef *GetBeepHandle(void)
{
	return &g_BeepDevice;
}

/*
*********************************************************************************************************
* Function Name : IO_Init
* Description	: IO��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IO_Init( void )
{
	InitBeep(&g_BeepDevice);
	
	ECHO(DEBUG_KERNEL_INIT, "IO��ʼ�� .......... OK");
}

/*
*********************************************************************************************************
* Function Name : IO_Handler
* Description	: IO������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void IO_Handler(IO_TypeDef *this)
{
	/* IO���� */
	{
		IO_OPERATE_TypeDef *inputHandle = GetIO_InputHandle();
		inputHandle->readSync(inputHandle);
		
		uint32_t inputReg = 0;
		uint32_t __reg = inputHandle->read(inputHandle);
			
		if (READ_BIT(__reg, BSP_BTN_BOOTRESET))
		{
			SET_BIT(inputReg, APP_IO_INPUT_MASK_NULL);
		}
		
		this->inputReg = inputReg;
	}
	
	/* IO��� */
	{
		IO_OPERATE_TypeDef *outputHandle = GetIO_OutputHandle();
		
	#if 0
		uint32_t outputReg = this->outputReg;
		
		if ( READ_BIT(outputReg, APP_IO_OUTPUT_MASK_BEEP) )
		{
			outputHandle->setOutputBit(outputHandle, BSP_DO_BEEP);
		}
		else
		{
			outputHandle->clrOutputBit(outputHandle, BSP_DO_BEEP);
		}
	#endif
	
		outputHandle->writeSync(outputHandle);
	}
	
	/* С���ź� */
	{
		IO_OPERATE_TypeDef *lampHandle = GetIO_LampHandle();
		
		uint32_t lamp = this->lampReg;
		
		if ( READ_BIT(lamp, APP_IO_LAMP_MASK_COMM) )
		{
			lampHandle->setOutputBit(lampHandle, BSP_LAMP_BOARD_COM);
		}
		else
		{
			lampHandle->clrOutputBit(lampHandle, BSP_LAMP_BOARD_COM);
		}
		
		if ( READ_BIT(lamp, APP_IO_LAMP_MASK_PUMP_OPEN) )
		{
			lampHandle->setOutputBit(lampHandle, BSP_LAMP_PUMP_START);
		}
		else
		{
			lampHandle->clrOutputBit(lampHandle, BSP_LAMP_PUMP_START);
		}
		
		if ( READ_BIT(lamp, APP_IO_LAMP_MASK_PUMP_CLOSE) )
		{
			lampHandle->setOutputBit(lampHandle, BSP_LAMP_PUMP_STOP);
		}
		else
		{
			lampHandle->clrOutputBit(lampHandle, BSP_LAMP_PUMP_STOP);
		}
		
		lampHandle->writeSync(lampHandle);
	}
}

/*
*********************************************************************************************************
* Function Name : GetIOHandle
* Description	: ��ȡIO���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
IO_TypeDef *GetIOHandle(void)
{
	return &g_IO_Device;
}

/*
*********************************************************************************************************
* Function Name : AppTaskIO
* Description	: IO����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskIO(void *p_arg)
{
	OS_ERR err;
	(void)p_arg;
	
	while (1)
	{	
		OSSchedLock(&err);	
				
		IO_Handler(&g_IO_Device);
		
		OSSchedUnlock(&err);
			
		BSP_OS_TimeDlyMs(SAMPLE_PERIOD);		
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
