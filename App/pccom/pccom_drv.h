/**
  ******************************************************************************
  * @文件名     ： nvidia_drv.h
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年03月01日
  * @摘要       ： CANOpen驱动头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _PCCOM_DRV_H
#define _PCCOM_DRV_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "bsp_usart.h"


/* 静态变量 ------------------------------------------------------------------*/
static xQueueHandle xPCRcvQueue = NULL;        //UART接收队列

/* 宏定义 --------------------------------------------------------------------*/
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
#define PC_COM_Priority        11                      //优先级
#define PC_COM_BaudRate        115200                   //波特率
#define PC_COM_IRQHandler      USART2_IRQHandler        //中断函数接口(见stm32f10x_it.c)

	  	
#define PC_COM_RX_QUEUE_LEN           10                       //Nvidia_com接收队列长度(队列的数量)(发送)
#define PC_COM_RX_QUEUE_SIZE          19                       //Nvidia_com接收队列大小(一个队列长度) - sizeof(CanTxMsg)



/* Nvidia serial msg */
/** 从英伟达板发送的数据，作为全局变量存储 **/
#define PC_COM_REC_LEN  			100  		//定义最大接收字节数 200
extern u8  PC_COM_RX_BUF[PC_COM_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 PC_COM_RX_STA;       				//接收状态标记
extern u8  PC_COM_RX_BUF_BACKUP[PC_COM_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符


/* 函数申明 ------------------------------------------------------------------*/
void PCcom_Driver_Init(void);
void PC_SendNByte(uint8_t *pData, uint16_t Length);
void PC_SendByte(uint8_t Data);

#endif /* _PCCOM_DRV_H */

/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
