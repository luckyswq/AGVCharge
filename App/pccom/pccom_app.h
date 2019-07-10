/**
  ******************************************************************************
  * @文件名     ： nvidia_drv.h
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年03月01日
  * @摘要       ： Nvidia com with arm应用程序头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _PCCOM_APP_H
#define _PCCOM_APP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "bsp_usart.h"
#include "stepper_msg.h"
#include "pccom_drv.h"

/* 宏定义 --------------------------------------------------------------------*/
#define PCCOM_STACK_SIZE        128                      //CANOPEN任务堆栈大小
#define PCCOM_TASK_PRIORITY     3                        //CANOPEN任务优先级


extern TaskHandle_t PCcom_App_Task_Handler;


/* 函数申明 ------------------------------------------------------------------*/
void PCcom_App_Init(void);

void PC_COM_IRQHandler(void);


#endif /* _PCCOM_APP_H */

/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
