/*
*********************************************************************************************************
* @file    	LinkDoubleCycleList.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-9 15:09:24
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	˫��ѭ������Դ�ļ�
*********************************************************************************************************
* @attention
*		˫������Ҳ��˫�����������һ�֣�����ÿ�����ݽ���ж�������ָ�룬�ֱ�ָ��ֱ�Ӻ�̺�ֱ��ǰ����
*	���ԣ���˫�������е�����һ����㿪ʼ�������Ժܷ���ط�������ǰ�����ͺ�̽�㡣һ�����Ƕ�����
*	˫��ѭ������
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkDoubleCycleList.h"


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
* Function Name : CreateLinkDoubleCycleList
* Description	: ����˫��ѭ������(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_DOUBLE_CYCLE_LIST_TypeDef *CreateLinkDoubleCycleList( void )
{
	/* ����ͷ�ڵ㣬ͷָ�����ⲿ�ṩ */
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead = (LINK_DOUBLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_DOUBLE_CYCLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->prev = pHead;
	pHead->next = pHead;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkDoubleCycleListIsEmpty
* Description	: ˫��ѭ�������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkDoubleCycleListIsEmpty( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if ((pHead->prev == pHead) &&\
		(pHead->next == pHead))
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
* Function Name : GetLinkDoubleCycleListLenth
* Description	: ��ȡ˫��ѭ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkDoubleCycleListLenth( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
	if (pHead != NULL)
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
		
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
* Function Name : ClearLinkDoubleCycleList
* Description	: ���˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
{
	if (pHead != NULL)
	{
		while (pHead->next != pHead)
		{
			LINK_DOUBLE_CYCLE_LIST_TypeDef *oldNode = pHead->next;
			pHead->next = oldNode->next;
			oldNode->next->prev = pHead;
			oldNode->next = NULL;
			oldNode->prev = NULL;
			
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
* Function Name : DestoryLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef **pHead )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *this = *pHead;
	
	if (this != NULL)
	{
		ClearLinkDoubleCycleList(this);
		free(this);		
		*pHead = NULL;
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkDoubleCycleList
* Description	: ˫��ѭ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
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
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = (LINK_DOUBLE_CYCLE_LIST_TypeDef *)calloc(1,sizeof(LINK_DOUBLE_CYCLE_LIST_TypeDef));
	if (pNode == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = prevNode->next;	
	pNode->prev = prevNode;
	pNode->next->prev = pNode;
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
* Function Name : DeleteLinkDoubleCycleList
* Description	: ˫��ѭ������ɾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
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
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *oldNode = prevNode->next;	
	prevNode->next = oldNode->next;
	oldNode->next->prev = prevNode;
	oldNode->next = NULL;
	oldNode->prev = NULL;
	
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
* Function Name : PushLinkDoubleCycleList
* Description	: ѹ��˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
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
* Function Name : PopLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
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
* Function Name : TraverseNextLinkDoubleCycleList
* Description	: ������˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseNextLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
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

/*
*********************************************************************************************************
* Function Name : TraversePrevLinkDoubleCycleList
* Description	: ��ǰ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraversePrevLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	while (pNode->prev != pHead)
	{
		pNode = pNode->prev;
		if (pNode->data == NULL)
		{
			return STATUS_DATA_STRUCT_FALSE;
		}
		show_CallBack(pNode->data);
	}
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SpliceLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SpliceLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pHeadNew )
{
	if ((pHead == NULL) || (pHeadNew == NULL))
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkDoubleCycleListIsEmpty(pHeadNew) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *first = pHeadNew->next;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *last = pHeadNew->prev;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *at = pHead->next;
	
	pHead->next = first;
	first->prev = pHead;
	at->prev = last;
	last->next = at;
	
	/* �½�㲢δ�ͷ�ͷ����ڴ棬��Ҫ�ⲿ free() */
	pHeadNew->next = pHeadNew;
	pHeadNew->prev = pHeadNew;
	
	return STATUS_DATA_STRUCT_TRUE;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
