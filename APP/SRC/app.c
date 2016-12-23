/*
*********************************************************************************************************
* @file    	app.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	Ӧ�ò�Դ�ļ�
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
#include "app.h"
#include "App_Menu.h"
#include "App_Language.h"

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

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
void APP_DebugViewTaskCreate( void );


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : APP_Init
* Description	: APP��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void APP_Init( void )
{
	ECHO(DEBUG_APP_INIT, "==================== APP ====================");
	
	
	
	ECHO(DEBUG_APP_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: _cbDesktop
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE * pMsg) 
{	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
		{	
			break;
		}
		case WM_PAINT:	
		{
			GUI_SetBkColor(MAIN_BKCOLOR);
			GUI_SetColor(MAIN_TEXTCOLOR);
			GUI_SetFont(MAIN_FONT);
			GUI_Clear();
			
			{
				int x, y, w, h;
				
				x = MAIN_BORDER;
				y = MAIN_TITLE_HEIGHT;
				w = FRAME_TOTAL_WIDTH - (MAIN_BORDER * 2);
				h = FRAME_TOTAL_HEIGHT - (MAIN_BORDER + MAIN_TITLE_HEIGHT);
				_DrawDownRect(FRAME_EFFECT, x, y, x + w - 1, y + h - 1);
			}
			break;
		}
		case WM_NOTIFY_PARENT:
			break;
			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*
*********************************************************************************************************
* Function Name : NotifyFlashFontProgress_CallBack
* Description	: ֪ͨ�ֿ���½���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void NotifyFlashFontProgress_CallBack( uint8_t fontSize, uint8_t progress )
{
	ECHO(DEBUG_APP_INIT, "font = %d, progress = %d\r\n",fontSize,progress);
}

/*
*********************************************************************************************************
* Function Name : AppTaskCreate
* Description	: ����Ӧ�ó�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
	App_MenuTaskCreate();
	
	while (1)
	{
		GUI_Delay(10);
	}
}

/*
*********************************************************************************************************
* Function Name : MainTask
* Description	: GUI���������е�GUI�����Ǹ���������ࣩ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	/* ��ֹ������˸ */	
	WM_SetCreateFlags(WM_CF_MEMDEV); 
	
	/* emWin ��ʼ�� */
	GUI_Init();  
	
	/* �����ֿ� */
	UpdateFlashFontTask(NotifyFlashFontProgress_CallBack);
		
	/* ����Ĭ��Ƥ�� */
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	
	/* ��������ص����� */
	WM_SetCallback(WM_HBKWIN, _cbDesktop);
	
	/* ��ʾ�α� */
#if 0
	GUI_CURSOR_Show();
	GUI_CURSOR_SetPosition(0,0);
	GUI_CURSOR_Select(&GUI_CursorArrowLI);
#endif	

	/* �������� */
	AppTaskCreate();
	
	while (1)
	{		
		GUI_Delay(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUI
* Description	: GUI����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskGUI(void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{
		MainTask();
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
