/***
	************************************************************************************************
	*	@file  	usart.c
	*	@version V1.0
	*  @date    2022-7-7
	*	@author  ���ͿƼ�
	*	@brief   usart��غ���
   ************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H750XBH6���İ� ���ͺţ�FK750M5-XBH6��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> �ļ�˵����
	*
	*  ��ʼ��usart���ţ����ò����ʵȲ���
	*
	************************************************************************************************
***/


#include "usart.h"
#include "stm32h7xx_hal.h"


extern UART_HandleTypeDef huart1;  // UART_HandleTypeDef �ṹ�����

/*************************************************************************************************
*	����Щ���ϣ�����LVGL��Ϊ��Ҫ��__aeabi_assert����TouchGFX�����ܹ�ѡ microLib ��ʹ��printf
*	�������´��룬�ñ�׼C��֧���ض���fput
*  ���ݱ�������ѡ���Ӧ�Ĵ��뼴��
*************************************************************************************************/


//// AC5������ʹ����δ���
//#pragma import(__use_no_semihosting)  
//int _ttywrch(int ch)    
//{
//    ch=ch;
//	return ch;
//}         
//struct __FILE 
//{ 
//	int handle; 

//}; 
//FILE __stdout;       

//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 



//// AC6������ʹ����δ���
//__asm (".global __use_no_semihosting\n\t");
//void _sys_exit(int x) 
//{
//  x = x;
//}
////__use_no_semihosting was requested, but _ttywrch was 
//void _ttywrch(int ch)
//{
//    ch = ch;
//}

//FILE __stdout;



/*************************************************************************************************
*	�� �� ��:	fputc
*	��ڲ���:	ch - Ҫ������ַ� ��  f - �ļ�ָ�루�����ò�����
*	�� �� ֵ:	����ʱ�����ַ�������ʱ���� EOF��-1��
*	��������:	�ض��� fputc ������Ŀ����ʹ�� printf ����
*	˵    ��:	��		
*************************************************************************************************/

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);	// ���͵��ֽ�����
	return (ch);
}
