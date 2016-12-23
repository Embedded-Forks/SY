/*
*********************************************************************************************************
* @file    	kernel.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	�ں�Դ�ļ�
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
#include "ff_gen_drv.h"
#include "emWinUtils.h"
#include "SeqQueue.h"
#include "SeqList.h"
#include "usbh_usr.h"
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
HID_Usr_State HID_HostState;

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
* Function Name : KERNEL_Init
* Description	: �ں˳�ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void KERNEL_Init( void )
{
	ECHO(DEBUG_KERNEL_INIT, "==================== KERNEL ====================");
	
	Key_Init();
	IO_Init();

	ECHO(DEBUG_KERNEL_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
*                              				ѭ������
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : RepeatExecuteTaskInit
* Description	: �ظ�ִ�������ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RepeatExecuteTaskInit( CYCLE_TASK_TypeDef *cycleTaskPtr, uint32_t runPeriod,
							void *devicePtr )
{
	RepeatExecuteTaskDeInit(cycleTaskPtr);
	cycleTaskPtr->status = STATUS_CYCLE_TASK_IDLE;
	cycleTaskPtr->runPeriod = runPeriod;
	cycleTaskPtr->devicePtr = devicePtr;
}
	
/*
*********************************************************************************************************
* Function Name : RepeatExecuteTaskDeInit
* Description	: �ظ�ִ������ָ�Ĭ��ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RepeatExecuteTaskDeInit( CYCLE_TASK_TypeDef *cycleTaskPtr )
{
	memset(cycleTaskPtr,NULL,sizeof(CYCLE_TASK_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : SetRepeatExecuteTime
* Description	: �����ظ�ִ������ʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SetRepeatExecuteTime( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		uint32_t executeNums, uint32_t onTimeMS, uint32_t offTimeMS )
{
	//�ȴ��ϴ�ִ�н���
	if (cycleTaskPtr->status == STATUS_CYCLE_TASK_IDLE)
	{
		cycleTaskPtr->enableFlag = true;
		cycleTaskPtr->executeNums = executeNums;
		cycleTaskPtr->onTime = onTimeMS;
		cycleTaskPtr->offTime = offTimeMS;
	}
}

/*
*********************************************************************************************************
* Function Name : CloseRepeatExecuteTask
* Description	: �ر��ظ�ִ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CloseRepeatExecuteTask( CYCLE_TASK_TypeDef *cycleTaskPtr )
{
	cycleTaskPtr->disableFlag = true;
	if (cycleTaskPtr->disableFlag == true)
	{
		cycleTaskPtr->disableFlag = false;
		cycleTaskPtr->enableFlag = false;
		cycleTaskPtr->status = STATUS_CYCLE_TASK_IDLE;
		if (cycleTaskPtr->offDevice_CallBack)
		{
			cycleTaskPtr->offDevice_CallBack(cycleTaskPtr->devicePtr);
		}
		if (cycleTaskPtr->taskDoneNotify_CallBack)
		{
			cycleTaskPtr->taskDoneNotify_CallBack(cycleTaskPtr->devicePtr);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : RegisterRepeatExecuteTaskStartNotify_CallBack
* Description	: ע���ظ�ִ������ʼ֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterRepeatExecuteTaskStartNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) )
{
	cycleTaskPtr->taskStartNotify_CallBack = taskStartNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterRepeatExecuteTaskOnDevice_CallBack
* Description	: ע��ѭ��ִ��������豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterRepeatExecuteTaskOnDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) )
{
	cycleTaskPtr->onDevice_CallBack = onDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterRepeatExecuteTaskOffDevice_CallBack
* Description	: ע��ѭ��ִ������ر��豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterRepeatExecuteTaskOffDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) )
{
	cycleTaskPtr->offDevice_CallBack = offDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterRepeatExecuteTaskDoneNotify_CallBack
* Description	: ע���ظ�ִ���������֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterRepeatExecuteTaskDoneNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) )
{
	cycleTaskPtr->taskDoneNotify_CallBack = taskDoneNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RepeatExecuteTaskCycle
* Description	: �ظ�ִ������ѭ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RepeatExecuteTaskCycle( CYCLE_TASK_TypeDef *cycleTaskPtr )
{	
	switch ( cycleTaskPtr->status )
	{
		case STATUS_CYCLE_TASK_IDLE:
		{
			if (cycleTaskPtr->enableFlag == true)			
			{
				cycleTaskPtr->enableFlag = false;
				
				if (cycleTaskPtr->runPeriod != 0)
				{
					if (cycleTaskPtr->executeNums != 0)
					{
						cycleTaskPtr->executeCount = 0;
						cycleTaskPtr->onCount = 0;
						cycleTaskPtr->offCount = 0;
						cycleTaskPtr->status = STATUS_CYCLE_TASK_ON;
						
						if (cycleTaskPtr->taskStartNotify_CallBack)
						{
							cycleTaskPtr->taskStartNotify_CallBack(cycleTaskPtr->devicePtr);
						}
					}
				}
			}
			break;
		}
		case STATUS_CYCLE_TASK_ON:
		{
			if (cycleTaskPtr->onDevice_CallBack)
			{
				cycleTaskPtr->onDevice_CallBack(cycleTaskPtr->devicePtr);
			}
			
			cycleTaskPtr->onCount++;
			if (cycleTaskPtr->onCount >= cycleTaskPtr->onTime / cycleTaskPtr->runPeriod)
			{
				cycleTaskPtr->onCount = 0;
				cycleTaskPtr->status = STATUS_CYCLE_TASK_OFF;
			}
			break;
		}
		case STATUS_CYCLE_TASK_OFF:
		{
			if (cycleTaskPtr->offDevice_CallBack)
			{
				cycleTaskPtr->offDevice_CallBack(cycleTaskPtr->devicePtr);
			}
			
			cycleTaskPtr->offCount++;
			if (cycleTaskPtr->offCount >= cycleTaskPtr->offTime / cycleTaskPtr->runPeriod)
			{
				cycleTaskPtr->offCount = 0;			
				
				cycleTaskPtr->executeCount++;
				if (cycleTaskPtr->executeCount >= cycleTaskPtr->executeNums)
				{
					cycleTaskPtr->executeCount = 0;

					cycleTaskPtr->status = STATUS_CYCLE_TASK_DONE;
				}
				else
				{
					cycleTaskPtr->status = STATUS_CYCLE_TASK_ON;
				}
			}
			break;
		}
		case STATUS_CYCLE_TASK_DONE:
		{
			if (cycleTaskPtr->offDevice_CallBack)
			{
				cycleTaskPtr->offDevice_CallBack(cycleTaskPtr->devicePtr);
			}
			if (cycleTaskPtr->taskDoneNotify_CallBack)
			{
				cycleTaskPtr->taskDoneNotify_CallBack(cycleTaskPtr->devicePtr);
			}
			cycleTaskPtr->status = STATUS_CYCLE_TASK_IDLE;
			break;
		}
		default:
			cycleTaskPtr->status = STATUS_CYCLE_TASK_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
*                              				��ʱֹͣ����
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : DelayStopTaskInit
* Description	: ��ʱֹͣ�����ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DelayStopTaskInit( DELAY_STOP_TASK_TypeDef *delayStopPtr, uint32_t runPeriod,
						void *devicePrt )
{
	DelayStopTaskDeInit(delayStopPtr);
	delayStopPtr->status = STATUS_DELAY_STOP_TASK_IDLE;
	delayStopPtr->runPeriod = runPeriod;
	delayStopPtr->devicePrt = devicePrt;
}

/*
*********************************************************************************************************
* Function Name : DelayStopTaskDeInit
* Description	: ��ʱֹͣ����ָ�Ĭ��ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DelayStopTaskDeInit( DELAY_STOP_TASK_TypeDef *delayStopPtr )
{
	memset(delayStopPtr,NULL,sizeof(DELAY_STOP_TASK_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : SetDelayStopTime
* Description	: ������ʱֹͣʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SetDelayStopTime( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		uint32_t delayTimeMS )
{
	delayStopPtr->enableFlag = true;
	delayStopPtr->status = STATUS_DELAY_STOP_TASK_IDLE;
	delayStopPtr->delayTime = delayTimeMS;
}

/*
*********************************************************************************************************
* Function Name : CloseDelayStopTask
* Description	: �ر���ʱֹͣ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CloseDelayStopTask( DELAY_STOP_TASK_TypeDef *delayStopPtr )
{
	delayStopPtr->disableFlag = true;
	if (delayStopPtr->disableFlag == true)
	{
		delayStopPtr->disableFlag = false;
		delayStopPtr->enableFlag = false;
		
		delayStopPtr->status = STATUS_DELAY_STOP_TASK_IDLE;		
		if (delayStopPtr->offDevice_CallBack)
		{
			delayStopPtr->offDevice_CallBack(delayStopPtr->devicePrt);
		}
		if (delayStopPtr->taskDoneNotify_CallBack)
		{
			delayStopPtr->taskDoneNotify_CallBack(delayStopPtr->devicePrt);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : IsDelayStopTaskRuning
* Description	: �Ƿ���ʱֹͣ������������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool IsDelayStopTaskRuning( DELAY_STOP_TASK_TypeDef *delayStopPtr )
{
	if (delayStopPtr->status == STATUS_DELAY_STOP_TASK_IDLE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
*********************************************************************************************************
* Function Name : RegisterDelayStopTaskStartNotify_CallBack
* Description	: ע����ʱֹͣ����ʼ֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterDelayStopTaskStartNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) )
{
	delayStopPtr->taskStartNotify_CallBack = taskStartNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterDelayStopTaskOnDevice_CallBack
* Description	: ע����ʱֹͣ������豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterDelayStopTaskOnDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) )
{
	delayStopPtr->onDevice_CallBack = onDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterDelayStopTaskOffDevice_CallBack
* Description	: ע����ʱֹͣ����ر��豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterDelayStopTaskOffDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) )
{
	delayStopPtr->offDevice_CallBack = offDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterDelayStopTaskDoneNotify_CallBack
* Description	: ע����ʱֹͣ�������֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterDelayStopTaskDoneNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) )
{
	delayStopPtr->taskDoneNotify_CallBack = taskDoneNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : DelayStopTaskCycle
* Description	: ��ʱֹͣ����ѭ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DelayStopTaskCycle( DELAY_STOP_TASK_TypeDef *delayStopPtr )
{
	switch ( delayStopPtr->status )
	{
		case STATUS_DELAY_STOP_TASK_IDLE:
		{
			if (delayStopPtr->enableFlag == true) 
			{
				delayStopPtr->enableFlag = false;
				
				if (delayStopPtr->runPeriod != 0)
				{
					if (delayStopPtr->delayTime != 0)
					{
						delayStopPtr->status = STATUS_DELAY_STOP_TASK_ON;
						
						if (delayStopPtr->taskStartNotify_CallBack)
						{
							delayStopPtr->taskStartNotify_CallBack(delayStopPtr->devicePrt);
						}
						delayStopPtr->delayCount = 0;
					}
				}
			}
			break;
		}
		case STATUS_DELAY_STOP_TASK_ON:
		{
			if (delayStopPtr->onDevice_CallBack)
			{
				delayStopPtr->onDevice_CallBack(delayStopPtr->devicePrt);
			}
			
			delayStopPtr->delayCount++;
			if (delayStopPtr->delayCount >= delayStopPtr->delayTime / delayStopPtr->runPeriod)
			{
				delayStopPtr->delayCount = 0;				
				delayStopPtr->status = STATUS_DELAY_STOP_TASK_DONE;
			}
			break;
		}
		case STATUS_DELAY_STOP_TASK_DONE:
		{
			if (delayStopPtr->offDevice_CallBack)
			{
				delayStopPtr->offDevice_CallBack(delayStopPtr->devicePrt);
			}
			if (delayStopPtr->taskDoneNotify_CallBack)
			{
				delayStopPtr->taskDoneNotify_CallBack(delayStopPtr->devicePrt);
			}
			delayStopPtr->status = STATUS_DELAY_STOP_TASK_IDLE;
			break;
		}
		default:
			delayStopPtr->status = STATUS_DELAY_STOP_TASK_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
*                              				IOģ��PWM����
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : PWM_IO_OutputInit
* Description	: IOģ��PWM�����ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PWM_IO_OutputInit( PWM_IO_TypeDef *PWM, float runPeriod, float outputPeriod, 
						uint16_t maxControlDegree, void *devicePrt )
{
	PWM_IO_OutputDeInit(PWM);	
	PWM->runPeriod = runPeriod;
	PWM->outputPeriod = outputPeriod;
	PWM->maxControlDegree = maxControlDegree;
	PWM->devicePrt = devicePrt;
}	

/*
*********************************************************************************************************
* Function Name : PWM_IO_OutputDeInit
* Description	: IOģ��PWM����ָ�Ĭ��ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PWM_IO_OutputDeInit( PWM_IO_TypeDef *PWM )
{
	memset(PWM,NULL,sizeof(PWM_IO_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : PWM_IO_SetOutput
* Description	: IOģ��PWM���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PWM_IO_SetOutput( PWM_IO_TypeDef *PWM, uint16_t dutyCycle )
{
	PWM->status = PWM_IO_STATUS_IDLE;
	PWM->dutyCycle = dutyCycle;
	PWM->enableFlag = true;
}

/*
*********************************************************************************************************
* Function Name : this
* Description	: �ر�IOģ��PWM���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PWM_IO_CloseOutput( PWM_IO_TypeDef *PWM )
{	
	PWM->disableFlag = true;
	if (PWM->disableFlag == true)
	{
		PWM->disableFlag = false;
		PWM->enableFlag = false;		
		PWM->continueRunFlag = false;
		PWM->status = PWM_IO_STATUS_IDLE;	
	
		if (PWM->offDevice_CallBack)
		{
			PWM->offDevice_CallBack(PWM->devicePrt);
		}	
		if (PWM->taskDoneNotify_CallBack)
		{
			PWM->taskDoneNotify_CallBack(PWM->devicePrt);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : RegisterPWM_IO_TaskStartNotify_CallBack
* Description	: ע��IOģ��PWM����ʼ֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterPWM_IO_TaskStartNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) )
{
	PWM_IO_Ptr->taskStartNotify_CallBack = taskStartNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterPWM_IO_TaskOnDevice_CallBack
* Description	: ע��IOģ��PWM������豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterPWM_IO_TaskOnDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*onDevice_CallBack)( void *devicePrt ) )
{
	PWM_IO_Ptr->onDevice_CallBack = onDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterPWM_IO_TaskOffDevice_CallBack
* Description	: ע��IOģ��PWM����ر��豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterPWM_IO_TaskOffDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*offDevice_CallBack)( void *devicePrt ) )
{
	PWM_IO_Ptr->offDevice_CallBack = offDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterPWM_IO_TaskDoneNotify_CallBack
* Description	: ע��IOģ��PWM�������֪ͨ�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterPWM_IO_TaskDoneNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) )
{
	PWM_IO_Ptr->taskDoneNotify_CallBack = taskDoneNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : PWM_IO_Output
* Description	: IOģ��PWM���ѭ���壨�����ڵ�����Ƶ�ʣ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PWM_IO_OutputCycle( PWM_IO_TypeDef *PWM )
{
	switch ( PWM->status )
	{
		case PWM_IO_STATUS_IDLE:
		{				
			if ((PWM->enableFlag==true) || (PWM->continueRunFlag==true))
			{
				PWM->enableFlag = false;
				PWM->continueRunFlag = false;
				PWM->PWM_Count = 0;
				PWM->sumCount = 0;
				PWM->Reg_CCR = 0;
				
				if (PWM->runPeriod)
				{
					PWM->sumCount = PWM->outputPeriod / PWM->runPeriod;
				}
				
				if (PWM->maxControlDegree)
				{
					PWM->Reg_CCR = (float)(PWM->dutyCycle) / PWM->maxControlDegree * PWM->sumCount;
				}
				
				if (PWM->Reg_CCR != 0)
				{
					PWM->status = PWM_IO_STATUS_OPEN;
				}
				else
				{
					PWM->status = PWM_IO_STATUS_CLOSE;
				}
			}
			
			if (PWM->status == PWM_IO_STATUS_OPEN)
			{
				if (PWM->taskStartNotify_CallBack)
				{
					PWM->taskStartNotify_CallBack(PWM->devicePrt);
				}
			}
			else if (PWM->status == PWM_IO_STATUS_CLOSE)
			{
				if (PWM->taskDoneNotify_CallBack)
				{
					PWM->taskDoneNotify_CallBack(PWM->devicePrt);
				}
			}
			break;
		}
		case PWM_IO_STATUS_OPEN:
		{		
			if (PWM->onDevice_CallBack)
			{
				PWM->onDevice_CallBack(PWM->devicePrt);
			}
			
			PWM->PWM_Count++;
			if (PWM->PWM_Count >= PWM->Reg_CCR)
			{
				if (PWM->PWM_Count >= PWM->sumCount)
				{
					PWM->status = PWM_IO_STATUS_DONE;
				}
				else
				{
					PWM->status = PWM_IO_STATUS_CLOSE;
				}
			}		
			break;
		}
		case PWM_IO_STATUS_CLOSE:
		{
			if (PWM->offDevice_CallBack)
			{
				PWM->offDevice_CallBack(PWM->devicePrt);
			}
			
			PWM->PWM_Count++;
			if (PWM->PWM_Count >= PWM->sumCount)
			{
				PWM->status = PWM_IO_STATUS_DONE;
			}
			break;
		}
		case PWM_IO_STATUS_DONE:
		{
			//ģ�¶�ʱ���������ظ�ִ�У�ֱ���յ�ָֹͣ�
			PWM->continueRunFlag = true;		
			PWM->status = PWM_IO_STATUS_IDLE;
			break;
		}
		default:
			PWM->status = PWM_IO_STATUS_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
*                              				��������
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : SwitchTaskInit
* Description	: ���������ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr, void *devicePrt )
{
	SwitchTaskDeInit(switchPtr);
	switchPtr->devicePrt = devicePrt;
}

/*
*********************************************************************************************************
* Function Name : SwitchTaskDeInit
* Description	: ��������ָ�Ĭ��ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SwitchTaskDeInit( SWITCH_TASK_TypeDef *switchPtr )
{
	memset(switchPtr,NULL,sizeof(SWITCH_TASK_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : RegisterSwitchTaskIsSwitchOn_CallBack
* Description	: ע�Ὺ�������Ƿ�򿪻ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterSwitchTaskIsSwitchOn_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		bool (*isSwitchOn_CallBack)( void *devicePrt ) )
{
	switchPtr->isSwitchOn_CallBack = isSwitchOn_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterSwitchTaskOnDevice_CallBack
* Description	: ע�Ὺ��������豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterSwitchTaskOnDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) )
{
	switchPtr->onDevice_CallBack = onDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : RegisterSwitchTaskOffDevice_CallBack
* Description	: ע�Ὺ������ر��豸�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterSwitchTaskOffDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) )
{
	switchPtr->offDevice_CallBack = offDevice_CallBack;
}

/*
*********************************************************************************************************
* Function Name : SwitchTask
* Description	: ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SwitchTask( SWITCH_TASK_TypeDef *switchPtr )
{
	switch ( switchPtr->status )
	{
		case SWITCH_TASK_IDLE:
		{
			if (switchPtr->isSwitchOn_CallBack)
			{
				if (switchPtr->isSwitchOn_CallBack(switchPtr->devicePrt) == true)
				{
					switchPtr->status = SWITCH_TASK_OPEN_NOTIFY;
				}
				else
				{
					switchPtr->status = SWITCH_TASK_CLOSE_NOTIFY;
				}
			}		
			break;
		}	
		case SWITCH_TASK_OPEN:
		{
			if (switchPtr->isSwitchOn_CallBack(switchPtr->devicePrt) == false)
			{
				switchPtr->status = SWITCH_TASK_CLOSE_NOTIFY;
			}
			break;
		}
		case SWITCH_TASK_OPEN_NOTIFY:
		{
			if (switchPtr->onDevice_CallBack)
			{
				switchPtr->onDevice_CallBack(switchPtr->devicePrt);
			}
			switchPtr->status = SWITCH_TASK_OPEN;
			break;
		}	
		case SWITCH_TASK_CLOSE:
		{
			if (switchPtr->isSwitchOn_CallBack(switchPtr->devicePrt) == true)
			{
				switchPtr->status = SWITCH_TASK_OPEN_NOTIFY;
			}
			break;
		}	
		case SWITCH_TASK_CLOSE_NOTIFY:
		{
			if (switchPtr->offDevice_CallBack)
			{
				switchPtr->offDevice_CallBack(switchPtr->devicePrt);
			}
			switchPtr->status = SWITCH_TASK_CLOSE;
			break;
		}
		default:
			switchPtr->status = SWITCH_TASK_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
*                              				���Ź�����
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : WatchDogTaskInit
* Description	: ���Ź������ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void WatchDogTaskInit( WATCH_DOG_TASK_TypeDef *wwdgPtr, uint32_t runPeriod, 
						void *devicePrt)
{
	WatchDogTaskDeInit(wwdgPtr);
	wwdgPtr->status = WATCH_DOG_TASK_IDLE;
	wwdgPtr->runPeriod = runPeriod;
	wwdgPtr->devicePrt = devicePrt;
}

/*
*********************************************************************************************************
* Function Name : WatchDogTaskDeInit
* Description	: ���Ź�����ָ�Ĭ��ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void WatchDogTaskDeInit( WATCH_DOG_TASK_TypeDef *wwdgPtr )
{
	memset(wwdgPtr,NULL,sizeof(WATCH_DOG_TASK_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : SetWatchDogCounter
* Description	: ���ÿ��Ź�������ֵ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SetWatchDogCounter( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		uint32_t countTimeMS )
{
	wwdgPtr->enableFlag = true;
	wwdgPtr->status = WATCH_DOG_TASK_IDLE;
	wwdgPtr->countTimeMS = countTimeMS;
}

/*
*********************************************************************************************************
* Function Name : CloseWatchDogTask
* Description	: �رտ��Ź�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CloseWatchDogTask( WATCH_DOG_TASK_TypeDef *wwdgPtr )
{
	wwdgPtr->disableFlag = true;
	if (wwdgPtr->disableFlag == true)
	{
		wwdgPtr->disableFlag = false;
		wwdgPtr->enableFlag = false;
		
		wwdgPtr->status = WATCH_DOG_TASK_IDLE;		
	}
}

/*
*********************************************************************************************************
* Function Name : RegisterWatchDogTaskTimeUpNotify_CallBack
* Description	: ע�ῴ�Ź�ʱ�䵽�ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void RegisterWatchDogTaskTimeUpNotify_CallBack( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		void (*timeUpNotify_CallBack)( void *devicePrt ) )
{
	wwdgPtr->timeUpNotify_CallBack = timeUpNotify_CallBack;
}

/*
*********************************************************************************************************
* Function Name : WatchDogTaskCycle
* Description	: ���Ź�����ѭ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void WatchDogTaskCycle( WATCH_DOG_TASK_TypeDef *wwdgPtr )
{
	switch ( wwdgPtr->status )
	{
		case WATCH_DOG_TASK_IDLE:
		{
			if (wwdgPtr->enableFlag == true) 
			{
				wwdgPtr->enableFlag = false;
				
				if (wwdgPtr->runPeriod)
				{
					wwdgPtr->decreaseCount = wwdgPtr->countTimeMS / wwdgPtr->runPeriod;
					wwdgPtr->status = WATCH_DOG_TASK_WORKING;
				}
			}
			break;
		}
		case WATCH_DOG_TASK_WORKING:
		{	
			if (wwdgPtr->decreaseCount)
			{
				wwdgPtr->decreaseCount--;
			}
			else
			{
				wwdgPtr->status = WATCH_DOG_TASK_DONE;
			}
			break;
		}
		case WATCH_DOG_TASK_DONE:
		{	
			if (wwdgPtr->timeUpNotify_CallBack)
			{
				wwdgPtr->timeUpNotify_CallBack(wwdgPtr->devicePrt);
			}
			wwdgPtr->status = WATCH_DOG_TASK_IDLE;
			break;
		}
		default:
			wwdgPtr->status = WATCH_DOG_TASK_IDLE;
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : HID_HostDetectTask
* Description	: ������̽����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HID_HostDetectTask( HID_Usr_State *hidHostPtr )
{
	USB_HOST_STATUS_TypeDef USB_HostStatus = USBH_GetStatus();
	
	switch (*hidHostPtr)
	{
		case HID_USR_IDLE:
			*hidHostPtr = HID_USR_WAIT;
			break;
		case HID_USR_WAIT:
			*hidHostPtr = HID_USR_START;
			break;
		case HID_USR_START:
		{
			if (USB_HostStatus == USB_HOST_READY)
			{
				HID_TypeTypeDef HID_Type = USBH_HID_GetDeviceType(&hUsbHostHS);
				if (HID_Type == HID_KEYBOARD)
				{
					*hidHostPtr = HID_USR_KEYBOARD;
					ECHO(DEBUG_KERNEL_HID, "��̽������...");
				}
				else if (HID_Type == HID_MOUSE)
				{
					*hidHostPtr = HID_USR_MOUSE;
					HID_MouseInit(g_mousePtr);
					ECHO(DEBUG_KERNEL_HID, "��̽�����...");
				}
				else
				{
					ECHO(DEBUG_KERNEL_HID, "δ֪�豸��");
				}
			}
			break;
		}
		case HID_USR_MOUSE:
			HID_MouseProcess(g_mousePtr);
			break;
		case HID_USR_KEYBOARD:
			
			break;
		case HID_USR_REENUMERATE:
			USBH_ReEnumerate(&hUsbHostHS);		
			*hidHostPtr = HID_USR_START;
			ECHO(DEBUG_KERNEL_HID, "����ö�٣�");
			break;
		default:
			*hidHostPtr = HID_USR_IDLE;
			break;
	}
	
	if (USB_HostStatus == USB_HOST_DISCONNECT)
	{
		*hidHostPtr = HID_USR_IDLE;
	}
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
