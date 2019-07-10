/**
  ******************************************************************************
  * @文件名     ： statusmanager_app.h
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年03月01日
  * @摘要       ： AGV状态管理与上传
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _LED_APP_H
#define _LED_APP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* 宏定义 --------------------------------------------------------------------*/
#define LED_STACK_SIZE 64
#define LED_TASK_PRIORITY 3

#define LED1_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED1_PIN                   GPIO_Pin_4
#define LED1_GPIO_PORT             GPIOA
#define LED2_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED2_PIN                   GPIO_Pin_5
#define LED2_GPIO_PORT             GPIOA

/* LED开关 */
#define LED1_ON()                  GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF()                 GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN)
#define LED1_TOGGLE()              (LED1_GPIO_PORT->ODR ^= LED1_PIN)
#define LED2_ON()                  GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN)
#define LED2_OFF()                 GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN)
#define LED2_TOGGLE()              (LED2_GPIO_PORT->ODR ^= LED2_PIN)

extern TaskHandle_t Led_App_Task_Handler;


/* 函数申明 ------------------------------------------------------------------*/
void Led_App_Init(void);



#endif /* _LED_APP_H */

/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
