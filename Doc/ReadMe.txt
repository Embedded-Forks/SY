============================================================================================================
																							HAL
============================================================================================================
ʹ��HAL���滻ԭ�б�׼�⺯������ע�����£�
	1.1 �������ļ��У���δ��ʼ��ϵͳʱ�ӣ���Ҫ����Ӧ�ò��ֶ���ʼ����	
	
	1.2 HAL����Ҫ�õ��δ�ʱ������ˣ��û�������ʹ�õδ�ʱ������Ϊ�û��㶨ʱ����
	
	1.3 �жϷ����� stm32f4xx_it.c/stm32f4xx_it.h����Ҫ�������ط�������HAL�ⲻ�ṩ��
	
	1.4 �����ļ���Ҫ�������ļ���Ϊstm32f4xx_hal_conf.h��
	
	1.5 uint32_t SystemCoreClock = 16000000; �ñ����� HAL_RCC_ClockConfig(���лᱻ�ı�Ϊ168000000;
	
	1.6 �ı������ж�������ƫ�����ķ�ʽ��ԭ���Ǻ��� NVIC_SetVectorTable()���ã����ڿ⺯��û���ṩ���API����˴�
		ԭ���Ŀ�����ֲ������
	
	1.7 �޸�ʱ�ӣ����ǵ��ⲿ����Ϊ25MHz��������Ĭ��Ϊ8MHz�������Ҫ�޸ģ�
		��1���޸��ļ� stm32f4xx_hal_conf.h��
			#if !defined  (HSE_VALUE) 
				#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
			#endif /* HSE_VALUE */
			��Ϊ
			#if !defined  (HSE_VALUE) 
				#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
			#endif /* HSE_VALUE */
		��2���޸� SystemClock_Config() ������
			*            HSE Frequency(Hz)              = 8000000
			*            PLL_M                          = 8
			*            PLL_N                          = 336
			*            PLL_P                          = 2
			*            PLL_Q                          = 7
			��Ϊ
			*            HSE Frequency(Hz)              = 25000000
			*            PLL_M                          = 25
			*            PLL_N                          = 336
			*            PLL_P                          = 2
			*            PLL_Q                          = 4
							
	1.8 ��ֲUSBʱ��U��С��ʼ��δ��������˵�������������Ա�STM32CubeMAX�����Ĵ��룬������PLL_Q���ô��󣬰�PLL_Q=4��ΪPLL_Q=7��
		�۲�USBָʾ��һֱ����˵��USB�Ѿ���������������ĳ��ԭ��δ����������
		���շ������� USBH_UserProcess()�����У����� f_mount()���������Һ�������Ϊ�������ط�ʽ��
	
	1.9 SD��U��
		��1��U�̰ε��Ժ���Ҫf_mount()
		��2��SD����U��f_mount���� 0:Do not mount (delayed mount)
		
	2.0 ��̫��	
		��1����̫�������ܵ������ݷ�������У�Ӧ�ó���Ӷ����ж�ȡ��
		��2����̫�����²��������Ҫʹ�� ethernetif_update_config()����
			
============================================================================================================
													emWin
============================================================================================================			
1��������ʾ
	��1��emWin�ĺ����ֿ�ʹ�á�FontCvtST.exe�����ɡ�
	��2�������ֿ���Դ����SD������Flash�����ô洢�ռ䡣
	��3��ͨ�������FontCvtST.exe�����ɵ��ֿ�ΪUnicode���뷽ʽ���ñ��뷽ʽ��UTF-8��UTF-16��UTF-32
			3�ֽ��뷽ʽ��ʹ���ֿ�֮ǰ��Ҫ���ú��� GUI_UC_SetEncodeUTF8(); ʹ��UTF-8���н��롣
			ͬʱ����������ʾ�ĺ��ֵġ�.c�ļ���Ҳ����ת��ΪUTF-8��ʽ��������������ʾ���֡�
	��4������keil5����BUG��UTF-8���ļ���Ҫʹ�ü��±����ΪUTF-8��ʽ��һ��ת���󣬲����ٱ��롣���򽫱��������Ҫ
		���±��档	
	��5��GUI_XBF_CreateFont() ��Ҫ���� GUI_Init()������أ�������ʾ�������롣


2������Ӳ���жϣ�
	��1��GUI_Init()��Ҫ����GUI�����У��ȵ�GUI������������ȥ��ʼ����
	
3��ʹ�ò���ϵͳ��emwin��Ҫ�������޸�
	��1��ʹ�ô���OS��Lib��
	��2��ʵ�� GUI_X�ڵ���غ���
	��3������GUIConf.h�ļ��У�#define GUI_OS              1
	
4�����ֿ����SD�������غ��ֺ�����
	��1�����Է��� f_lseek() ÿһ�ε��ö�����1ms����ʱ�䡣
	��2�����ֲ��ùٷ�������ɵ��ֿ�Ϊunicode���뷽����ÿ�ζ�λһ�����֣���Ҫ�ƶ�����ļ�ָ�룬ÿ�ζ�ȡ���ֽڲ�����4
		��������ʹ�ٶ��᲻��ȥ��
	��3��ͨ����ֲ����������������������֧�ּ���gb2312���ֿ�
	��4��ͨ���ѳ��ú����ֿ��������flash��Ӣ���ֿ��������flash��ʵ��������ټ��ء�
	
5��emWin��ôʹ��GB2312���ֿ⣿
	��1������ļ� GUI_UC_EncodeNone.c��GUI_UC_FontConfig.c��GUI_UC_X.c �ļ�
	��2������ʹ�ú��� GUI_UC_SetEncodeUTF8();
	��3��ʹ�ú��� MULTIEDIT_SetFont(hItem, &GUI_FontHZ32); ��������
	��4������ʾ���ı���ʽ������Ϊ UTF-8 ����
		
			
============================================================================================================
													LCD
============================================================================================================
1��	/* ����Ӳ����λ��������Ļ���ܴ���ƫ�� */
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_RESET);
	bsp_DelayMS(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_SET);
	bsp_DelayMS(20);
	
============================================================================================================
													LwIP
============================================================================================================
1��	/* ����Ӳ����ʼ�� */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);

============================================================================================================
													uCos-III
============================================================================================================
1��	�޸� startup_stm32f407xx.s �ļ���
	��1���� PendSVHandler �޸�Ϊ OS_CPU_PendSVHandler
	��2���� SysTickHandler �޸�Ϊ OS_CPU_SysTickHandler

2��	��д HAL_InitTick() ������ִ�� uCOS-III �ڲ��ĵδ�ʱ����ʼ������
3��	�ڹ��Ӻ��� App_OS_TimeTickHook()�������� HAL_SYSTICK_IRQHandler()
4�� ���Ը�����ʱ������Ӳ���жϡ��ٷ���ֲ�Ĺ�����MDK4.7 ���°汾�п������������У��������ϰ汾�������С���˲��ð�����
	�ķ�ʽ��ֲ��
	��1���޸ĺ��� OSTaskStkInit()��OS_CPU_PendSVHandler()
	��2��ʵ����ֲ���������ļ��󣬲����л�����ֻ���ڿ���������ִ�С���ˣ���ʱ�ȹر�Ӳ��FPU����������취��
5��	�޸ĺ��� BSP_OS_TimeDlyMs()����ʱ����
	ms  = dly_ms / 1000u;
    sec = dly_ms % 1000u;
	��Ϊ��
	ms  = dly_ms % 1000u;
    sec = dly_ms / 1000u;
6��	ʹ������ԭ�ӵ���ֲ��ʽ�����Խ������FPU����Ӳ���жϵ����⣺
	��1���޸������ļ�startup_stm32f407xx.s���޸ģ�
	IMPORT  __main

			 LDR     R0, =SystemInit
			 BLX     R0		 
			 LDR     R0, =__main
			 BX      R0
			 ENDP
			 
	Ϊ��
	IMPORT  __main

			 LDR     R0, =SystemInit
			 BLX     R0
			 
			 IF {FPU} != "SoftVFP"
											; Enable Floating Point Support at reset for FPU
			 LDR.W   R0, =0xE000ED88         ; Load address of CPACR register
			 LDR     R1, [R0]                ; Read value at CPACR
			 ORR     R1,  R1, #(0xF <<20)    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
											; Write back the modified CPACR value
			 STR     R1, [R0]                ; Wait for store to complete
			 DSB
			
											; Disable automatic FP register content
											; Disable lazy context switch
			 LDR.W   R0, =0xE000EF34         ; Load address to FPCCR register
			 LDR     R1, [R0]
			 AND     R1,  R1, #(0x3FFFFFFF)  ; Clear the LSPEN and ASPEN bits
			 STR     R1, [R0]
			 ISB                             ; Reset pipeline now the FPU is enabled
			 ENDIF
			 
			 LDR     R0, =__main
			 BX      R0
			 ENDP
			 
	��2������Ҫ�޸�  OSTaskStkInit()��OS_CPU_PendSVHandler()
	��3����"Target-Float Point HardWare"��ʹ��FPU��
	��4����ÿ�����񴴽�ʱ������Ҫ��� OS_OPT_TASK_SAVE_FP ѡ�
	��5��������ȷִ�и�������
	��6��ԭ�ӵĽ��ͣ���CORTEX-M4�ں��и� LAazy Stacking���ܣ����ʹ��FPU���ܵĻ���Ҫ�ر�������ܡ�
	��7��FPUֻ�Ե����ȸ��������㣬�ٶȺܿ졣����˫���ȸ��������ǲ�����ͨ���㷽ʽ��
7��	���⣺ʹ��FatFs��ȡSD��ʱ������ FR_NOT_READY ״̬��
	�������1����� syscall.c �ļ�
		  ��2���� _FS_REENTRANT ѡ�� 
		  ��3��GUI_Init();��ʼ����GUI������������ų�ʼ�����������Ӳ���ж�
		  ��4��SD�������У�SD�ж����ȼ�һ��Ҫ����SD_DMA�ж����ȼ����������ж���һֱ�ȴ�DMA״̬
		  
============================================================================================================
													USB
============================================================================================================
1��	���⣺��uCOS-III�����£�ʹ��USB��д���ݣ����ء����̴��󡱡�
	ԭ��USB��ȡ����ʱ��һֱ���ڵȴ�״̬��ԭ���Ƕ������л������µȴ���ʱ��
	�������USB�ж����ȼ�����Ϊ��ߣ�������ʱ�����
	
2�� ���⣺��uCOS-III�����£��γ�U�̺�ʹ��f_mount(NULL,...); ���ش���״̬��
	ԭ��FATFS���ú��� ff_del_syncobj() ����ʧ�ܡ�
	������ο�����������δ�ε�U�̲�δ���øú��������Ҳ���ٵ��á�
	
	

	
�
