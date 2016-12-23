/*
*********************************************************************************************************
* @file    	kernel.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	�ں�ͷ�ļ�
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __KERNEL_H
#define __KERNEL_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "bsp.h"
#include "key.h"
#include "io.h"
#include "date.h"
#include "GUI.h"
#include "DIALOG.h"
#include "mouse.h"

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef bool (*UART_SEND_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf, uint16_t _usLen );
typedef bool (*UART_RECEIVE_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf );


/*
*********************************************************************************************************
*                              				ѭ������
*********************************************************************************************************
*/
typedef enum
{
	STATUS_CYCLE_TASK_IDLE = 0,
	STATUS_CYCLE_TASK_ON,
	STATUS_CYCLE_TASK_OFF,
	STATUS_CYCLE_TASK_DONE,
}CYCLE_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	CYCLE_TASK_STATUS_TypeDef status;					//״̬
	uint32_t runPeriod;									//������������(��λ��ms)
	void *devicePtr;									//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );		//���豸
	void (*offDevice_CallBack)( void *devicePrt );		//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;							//ʹ�ܿ���
	bool disableFlag;							//ʧ�ܿ���
	uint32_t executeNums;						//ִ�д���
	uint32_t onTime;							//��ʱ��
	uint32_t offTime;							//�ر�ʱ��
	uint32_t executeCount;						//ִ�ж��������
	uint32_t onCount;							//�򿪼�����
	uint32_t offCount;							//�رռ�����
}CYCLE_TASK_TypeDef;	

/*
*********************************************************************************************************
*                              				��ʱֹͣ����
*********************************************************************************************************
*/
typedef enum
{
	STATUS_DELAY_STOP_TASK_IDLE = 0,
	STATUS_DELAY_STOP_TASK_ON,
	STATUS_DELAY_STOP_TASK_DONE,
}DELAY_STOP_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	DELAY_STOP_STATUS_TypeDef status;						//״̬
	uint32_t runPeriod;										//������������(��λ��ms)
	void *devicePrt;										//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );	//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );			//���豸
	void (*offDevice_CallBack)( void *devicePrt );			//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );		//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;							//ʹ�ܿ���
	bool disableFlag;							//ʧ�ܿ���
	uint32_t delayTime;							//��ʱʱ��
	uint32_t delayCount;						//��ʱ������
}DELAY_STOP_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				IOģ��PWM����
*********************************************************************************************************
*/
typedef enum
{
	PWM_IO_STATUS_IDLE = 0,
	PWM_IO_STATUS_OPEN,
	PWM_IO_STATUS_CLOSE,
	PWM_IO_STATUS_DONE,
}PWM_IO_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	PWM_IO_STATUS_TypeDef status;					//״̬
	float runPeriod;								//��������
	float outputPeriod;								//�������
	uint16_t maxControlDegree;						//��������
	void *devicePrt;								//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );		//���豸
	void (*offDevice_CallBack)( void *devicePrt );		//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;								//ʹ�ܿ���
	bool disableFlag;								//ʧ�ܿ���
	bool continueRunFlag;							//�������п���
	uint16_t dutyCycle;								//ռ�ձ�
	uint32_t PWM_Count;								//PWM������
	uint32_t sumCount;								//��ִ�д���
	uint32_t Reg_CCR;								//�Ƚϼ�����
}PWM_IO_TypeDef;

/*
*********************************************************************************************************
*                              				��������
*********************************************************************************************************
*/
typedef enum
{
	SWITCH_TASK_IDLE = 0,
	SWITCH_TASK_OPEN,
	SWITCH_TASK_OPEN_NOTIFY,
	SWITCH_TASK_CLOSE,
	SWITCH_TASK_CLOSE_NOTIFY,
}SWITCH_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	SWITCH_TASK_STATUS_TypeDef status;					//״̬
	void *devicePrt;									//�豸ָ��
	
	/* �ص����� */
	bool (*isSwitchOn_CallBack)( void *devicePrt );		//�����Ƿ��
	void (*onDevice_CallBack)( void *devicePrt );		//�򿪿���֪ͨ
	void (*offDevice_CallBack)( void *devicePrt );		//�رտ���֪ͨ
	
	/* �ɱ�������ʵ�� */	
}SWITCH_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				���Ź�����
*********************************************************************************************************
*/
typedef enum
{
	WATCH_DOG_TASK_IDLE = 0,
	WATCH_DOG_TASK_WORKING,
	WATCH_DOG_TASK_DONE,
}WATCH_DOG_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	WATCH_DOG_TASK_STATUS_TypeDef status;			//״̬
	uint32_t runPeriod;								//��������
	void *devicePrt;								//�豸ָ��
	
	/* �ص����� */
	void (*timeUpNotify_CallBack)( void *devicePrt );//ʱ�䵽��֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;								//ʹ�ܿ���
	bool disableFlag;								//ʧ�ܿ���
	uint32_t decreaseCount;							//�ݼ�������
	uint32_t countTimeMS;							//����ʱ��
}WATCH_DOG_TASK_TypeDef;

typedef enum {
	HID_USR_IDLE = 0,
	HID_USR_WAIT,  
	HID_USR_START,
	HID_USR_MOUSE,  
	HID_USR_KEYBOARD,    
	HID_USR_REENUMERATE,
}HID_Usr_State;

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/
extern HID_Usr_State HID_HostState;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void KERNEL_Init( void );

/*
*********************************************************************************************************
*                              				ѭ������
*********************************************************************************************************
*/
void RepeatExecuteTaskInit( CYCLE_TASK_TypeDef *cycleTaskPtr, uint32_t runPeriod,
							void *devicePtr );
void RepeatExecuteTaskDeInit( CYCLE_TASK_TypeDef *cycleTaskPtr );
void SetRepeatExecuteTime( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		uint32_t executeNums, uint32_t onTimeMS, uint32_t offTimeMS );
void CloseRepeatExecuteTask( CYCLE_TASK_TypeDef *cycleTaskPtr );	
void RepeatExecuteTaskCycle( CYCLE_TASK_TypeDef *cycleTaskPtr );
void RegisterRepeatExecuteTaskStartNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOnDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOffDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskDoneNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				��ʱֹͣ����
*********************************************************************************************************
*/
void DelayStopTaskInit( DELAY_STOP_TASK_TypeDef *delayStopPtr, uint32_t runPeriod,
						void *devicePrt );
void DelayStopTaskDeInit( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void SetDelayStopTime( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		uint32_t delayTimeMS );
void CloseDelayStopTask( DELAY_STOP_TASK_TypeDef *delayStopPtr );
bool IsDelayStopTaskRuning( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void DelayStopTaskCycle( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void RegisterDelayStopTaskStartNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOnDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOffDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskDoneNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				IOģ��PWM����
*********************************************************************************************************
*/
void PWM_IO_OutputInit( PWM_IO_TypeDef *PWM, float runPeriod, float outputPeriod, 
						uint16_t maxControlDegree, void *devicePrt );
void PWM_IO_OutputDeInit( PWM_IO_TypeDef *PWM );
void PWM_IO_SetOutput( PWM_IO_TypeDef *PWM, uint16_t dutyCycle );
void PWM_IO_CloseOutput( PWM_IO_TypeDef *PWM );
void PWM_IO_OutputCycle( PWM_IO_TypeDef *PWM_ObjPtr );
void RegisterPWM_IO_TaskStartNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOnDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOffDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskDoneNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				��������
*********************************************************************************************************
*/
void SwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr, void *devicePrt );
void SwitchTaskDeInit( SWITCH_TASK_TypeDef *switchPtr );
void SwitchTask( SWITCH_TASK_TypeDef *switchPtr );
void RegisterSwitchTaskIsSwitchOn_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		bool (*isSwitchOn_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOnDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOffDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
		
/*
*********************************************************************************************************
*                              				���Ź�����
*********************************************************************************************************
*/
void WatchDogTaskInit( WATCH_DOG_TASK_TypeDef *wwdgPtr, uint32_t runPeriod, 
						void *devicePrt);
void WatchDogTaskDeInit( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void SetWatchDogCounter( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		uint32_t countTimeMS );
void CloseWatchDogTask( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void RegisterWatchDogTaskTimeUpNotify_CallBack( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		void (*timeUpNotify_CallBack)( void *devicePrt ) );
void WatchDogTaskCycle( WATCH_DOG_TASK_TypeDef *wwdgPtr );

void HID_HostDetectTask( HID_Usr_State *hidHostPtr );


/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/



#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
