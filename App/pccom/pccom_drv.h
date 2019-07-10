/**
  ******************************************************************************
  * @�ļ���     �� nvidia_drv.h
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��01��
  * @ժҪ       �� CANOpen����ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _PCCOM_DRV_H
#define _PCCOM_DRV_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "bsp_usart.h"


/* ��̬���� ------------------------------------------------------------------*/
static xQueueHandle xPCRcvQueue = NULL;        //UART���ն���

/* �궨�� --------------------------------------------------------------------*/
/* Nvidia */
#define PC_COM                 USART2
#define PC_COM_CLK             RCC_APB1Periph_USART2
#define PC_COM_TX_GPIO_CLK     RCC_APB2Periph_GPIOA     //UART TX
#define PC_COM_TX_PIN          GPIO_Pin_2
#define PC_COM_TX_GPIO_PORT    GPIOA
#define PC_COM_RX_GPIO_CLK     RCC_APB2Periph_GPIOA     //UART RX
#define PC_COM_RX_PIN          GPIO_Pin_3
#define PC_COM_RX_GPIO_PORT    GPIOA
#define PC_COM_IRQn            USART2_IRQn
#define PC_COM_Priority        11                      //���ȼ�
#define PC_COM_BaudRate        115200                   //������
#define PC_COM_IRQHandler      USART2_IRQHandler        //�жϺ����ӿ�(��stm32f10x_it.c)

	  	
#define PC_COM_RX_QUEUE_LEN           10                       //Nvidia_com���ն��г���(���е�����)(����)
#define PC_COM_RX_QUEUE_SIZE          19                       //Nvidia_com���ն��д�С(һ�����г���) - sizeof(CanTxMsg)



/* Nvidia serial msg */
/** ��Ӣΰ��巢�͵����ݣ���Ϊȫ�ֱ����洢 **/
#define PC_COM_REC_LEN  			100  		//�����������ֽ��� 200
extern u8  PC_COM_RX_BUF[PC_COM_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 PC_COM_RX_STA;       				//����״̬���
extern u8  PC_COM_RX_BUF_BACKUP[PC_COM_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�


/* �������� ------------------------------------------------------------------*/
void PCcom_Driver_Init(void);
void PC_SendNByte(uint8_t *pData, uint16_t Length);
void PC_SendByte(uint8_t Data);

#endif /* _PCCOM_DRV_H */

/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
