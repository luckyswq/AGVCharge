#ifndef __STEPPER_DRV_H
#define __STEPPER_DRV_H
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "stepper_msg.h"
#include "bsp_usart.h"
/* 宏定义 --------------------------------------------------------------------*/
/* DEBUG */
#define STEPPER_COM                 USART3
#define STEPPER_COM_CLK             RCC_APB1Periph_USART3
#define STEPPER_COM_TX_GPIO_CLK     RCC_APB2Periph_GPIOB     //UART TX
#define STEPPER_COM_TX_PIN          GPIO_Pin_10
#define STEPPER_COM_TX_GPIO_PORT    GPIOB
#define STEPPER_COM_RX_GPIO_CLK     RCC_APB2Periph_GPIOB     //UART RX
#define STEPPER_COM_RX_PIN          GPIO_Pin_11
#define STEPPER_COM_RX_GPIO_PORT    GPIOB
#define STEPPER_COM_IRQn            USART3_IRQn
#define STEPPER_COM_Priority        12                       //优先级
#define STEPPER_COM_BaudRate        115200                   //波特率
#define STEPPER_COM_IRQHandler      USART3_IRQHandler        //中断函数接口(见stm32f10x_it.c)


void stepper_test(void);
void UART_Stepper_Configuration(void);


#endif

