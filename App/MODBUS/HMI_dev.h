#ifndef __HMI_DEV_H
#define __HMI_DEV_H
#include "stm32f10x.h"
#define HMI_COM_IRQHandler      USART2_IRQHandler        //�жϺ����ӿ�(��stm32f10x_it.c)
extern unsigned int HMITimerOut;						//������ͨ�ų�ʱ

void SendHMIData(USART_TypeDef* USARTx,unsigned char *puchMsg,unsigned int usDataLen);
void UART_HMI_Configuration(u32 bound);
void AGVSendDataToAppUART(USART_TypeDef* USARTx,unsigned char *puchMsg,unsigned int usDataLen);
void UART_App_Configuration(u32 bound);
#endif
