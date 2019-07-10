/**
  ******************************************************************************
  * @文件名     ： bsp.h
  * @作者       ： Felix Wu
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： 底层驱动头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"


/* 宏定义 --------------------------------------------------------------------*/
#define LED_GPIO_CLK              RCC_APB2Periph_GPIOC
#define LED_PIN                   GPIO_Pin_13
#define LED_GPIO_PORT             GPIOC

/* LED开关 */
#define LED_ON()                  GPIO_SetBits(LED_GPIO_PORT, LED_PIN)
#define LED_OFF()                 GPIO_ResetBits(LED_GPIO_PORT, LED_PIN)
#define LED_TOGGLE()              (LED_GPIO_PORT->ODR ^= LED_PIN)


/* 函数申明 ------------------------------------------------------------------*/
void BSP_Init(void);


#endif /* _BSP_H */

/**** Copyright (C)2018 Felix Wu. All Rights Reserved **** END OF FILE ****/
