/*
*********************************************************************************************************
* @file    	LinkSingleCycleList.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-9 13:49:34
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	����ѭ������Դ�ļ�
*********************************************************************************************************
* @attention
*		����ѭ�������ǵ������һ�ָĽ����������������β�ڵ��������㹹�ɵ���ѭ������ṹ��
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkSingleCycleList.h"


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

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : CreateLinkSingleCycleList
* Description	: ��������ѭ������(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_SINGLE_CYCLE_LIST_TypeDef *CreateLinkSingleCycleList( void )
{
	/* ����ͷ�ڵ㣬ͷָ�����ⲿ�ṩ */
	LINK_SINGLE_CYCLE_LIST_TypeDef *pHead = (LINK_SINGLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_CYCLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->next = pHead;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkSingleCycleListIsEmpty
* Description	: ����ѭ�������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkSingleCycleListIsEmpty( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == pHead)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : GetLinkSingleCycleListLenth
* Description	: ��ȡ����ѭ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkSingleCycleListLenth( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
	if (pHead != NULL)
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead;
		
		while (pNode->next != pHead)
		{
			lenth++;
			pNode = pNode->next;
		}
	}
	
	return lenth;
}

/*
*********************************************************************************************************
* Function Name : ClearLinkSingleCycleList
* Description	: ��յ���ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead != NULL)
	{
		while (pHead->next != pHead)
		{
			LINK_SINGLE_CYCLE_LIST_TypeDef *oldNode = pHead->next;
			pHead->next = oldNode->next;
			oldNode->next = NULL;
			
			if (oldNode->data != NULL)
			{
				free(oldNode->data);
				oldNode->data = NULL;
			}
			
			free(oldNode);
			oldNode = NULL;				
		}
	}
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkSingleCycleList
* Description	: ���ٵ���ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef **pHead )
{
	LINK_SINGLE_CYCLE_LIST_TypeDef *this = *pHead;
	
	if (this != NULL)
	{
		ClearLinkSingleCycleList(this);		
		free(this);		
		*pHead = NULL;
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleCycleList
* Description	: ����ѭ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	/* �����½ڵ� */
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = (LINK_SINGLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_SINGLE_CYCLE_LIST_TypeDef));
	if (pNode == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = prevNode->next;	
	prevNode->next = pNode;
	
	/* �洢�û����� */
	pNode->data = (void *)calloc(1,dataSize);
	if (pNode->data == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	memcpy(pNode->data,dataIn,dataSize);
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : DeleteLinkSingleCycleList
* Description	: ����ѭ������ɾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				isFind = true;
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *oldNode = prevNode->next;	
	prevNode->next = oldNode->next;
	oldNode->next = NULL;

	if (oldNode->data != NULL)
	{
		free(oldNode->data);
		oldNode->data = NULL;
	}
	free(oldNode);
	oldNode = NULL;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PushLinkSingleCycleList
* Description	: ѹ�뵥��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(pNode->data,dataIn,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : PopLinkSingleCycleList
* Description	: ��������ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : ReverseLinkSingleCycleList
* Description	: ������ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == NULL)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode1 = pHead->next;
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode2 = pNode1->next;
	
	pNode1->next = pHead;
	pNode1 = pNode2;
	pNode2 = pNode2->next;
	while (pNode1 != pHead)
	{
		pNode1->next = pHead->next;
		pHead->next = pNode1;
		
		pNode1 = pNode2;
		pNode2 = pNode2->next;
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkSingleCycleList
* Description	: ��������ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleCycleList( LINK_SINGLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_SINGLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	while (pNode->next != pHead)
	{
		pNode = pNode->next;
		if (pNode->data == NULL)
		{
			return STATUS_DATA_STRUCT_FALSE;
		}
		show_CallBack(pNode->data);
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
