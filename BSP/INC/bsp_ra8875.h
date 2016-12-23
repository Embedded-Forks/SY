/*
*********************************************************************************************************
* @file    	bsp_ra8875.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-20 09:53:57
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	RA8875����ͷ�ļ�
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
#ifndef __BSP_RA8875_H
#define __BSP_RA8875_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define	RA_HARD_8080_16_EN
#define	RA_HARD_SPI_EN

#define RA8875_BASE		((uint32_t)(0x60000000 | 0x00000000))
#define RA8875_RAM_ADDR	RA8875_BASE
#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  (1 << (16 + 1)))	/* FSMC 16λ����ģʽ�£�FSMC_A16���߶�Ӧ�����ַA17 */
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE)

#ifdef RA_HARD_SPI_EN	/* Ӳ�� SPI ���� (��Ҫ�ı�RA8875���ϵ�2������λ��) */
	/*
	��1��������STM32-V5������,STM32F407IGT6
		PD3/LCD_BUSY		--- ����оƬæ       ��RA8875����RA8875оƬ��æ�ź�)
		PF6/LCD_PWM			--- LCD����PWM����  ��RA8875������˽ţ�������RA8875����)

		PI10/TP_NCS			--- ����оƬ��Ƭѡ		(RA8875������SPI�ӿڴ���оƬ��
		PB3/SPI3_SCK		--- ����оƬSPIʱ��		(RA8875������SPI�ӿڴ���оƬ��
		PB4/SPI3_MISO		--- ����оƬSPI������MISO(RA8875������SPI�ӿڴ���оƬ��
		PB5/SPI3_MOSI		--- ����оƬSPI������MOSI(RA8875������SPI�ӿڴ���оƬ��

		PC3/TP_INT			--- ����оƬ�ж� ������RA8875������RA8875������ж�)
	*/
	#define RA8875_CS_0()	( (void)0 )
	#define RA8875_CS_1()	( (void)0 )

	#define SPI_WRITE_DATA	0x00
	#define SPI_READ_DATA	0x40
	#define SPI_WRITE_CMD	0x80
	#define SPI_READ_STATUS	0xC0

	static uint8_t SPI_ShiftByte(uint8_t _ucByte);
#endif

	
/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
/* ����RA8875�ķ��ʽӿ����. V5�����֧�� RA_HARD_SPI �� RA_HARD_8080_16 */
enum
{
	RA_SOFT_8080_8 = 0,	/* ���8080�ӿ�ģʽ, 8bit */
	RA_SOFT_SPI,	   	/* ���SPI�ӿ�ģʽ */
	RA_HARD_SPI,	   	/* Ӳ��SPI�ӿ�ģʽ */
	RA_HARD_8080_16		/* Ӳ��8080�ӿ�,16bit */
};

enum
{
	RA_FONT_16 = 0,		/* RA8875 ���� 16���� */
	RA_FONT_24 = 1,		/* RA8875 ���� 24���� */
	RA_FONT_32 = 2		/* RA8875 ���� 32���� */
};

/* ���ַŴ���� */
enum
{
	RA_SIZE_X1 = 0,		/* ԭʼ��С */
	RA_SIZE_X2 = 1,		/* �Ŵ�2�� */
	RA_SIZE_X3 = 2,		/* �Ŵ�3�� */
	RA_SIZE_X4 = 3		/* �Ŵ�4�� */
};

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

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void RA8875_Delaly1us(void);
void RA8875_Delaly1ms(void);
uint16_t RA8875_ReadID(void);
void RA8875_WriteCmd(uint8_t _ucRegAddr);
void RA8875_WriteData(uint8_t _ucRegValue);
uint8_t RA8875_ReadData(void);
void RA8875_WriteData16(uint16_t _usRGB);
uint16_t RA8875_ReadData16(void);
uint8_t RA8875_ReadStatus(void);
uint32_t RA8875_GetDispMemAddr(void);
uint8_t RA8875_ReadBusy(void);

/* �ɹ��ⲿģ����õĺ��� */
void RA8875_InitHard(void);
void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
uint8_t RA8875_ReadReg(uint8_t _ucRegAddr);
uint16_t RA8875_ReadID(void);
uint8_t RA8875_IsBusy(void);
void RA8875_Layer1Visable(void);
void RA8875_Layer2Visable(void);
void RA8875_DispOn(void);
void RA8875_DispOff(void);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize);
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_ClrScr(uint16_t _usColor);
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitWinMode(void);
void RA8875_CtrlGPO(uint8_t _pin, uint8_t _value);
void RA8875_SetBackLight(uint8_t _bright);

void RA8875_TouchInit(void);
uint16_t RA8875_TouchReadX(void);
uint16_t RA8875_TouchReadY(void);

void RA8875_InitSPI(void);
void RA8875_Sleep(void);

/* ����3����������ֱ��д�Դ棬��������ͷֱ��DMA����ʾ��. ��֧��SPI�ӿ�ģʽ */
void RA8875_StartDirectDraw(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitDirectDraw(void);

void RA8875_SetDirection(uint8_t _ucDir);

/* ����emWin�Ľӿں��� */
void RA8875_PutPixelGUI(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixelGUI(uint16_t _usX, uint16_t _usY);
void RA8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void RA8875_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor);
void RA8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_RTERect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t _usColor); 


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
