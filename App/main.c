/**
  ******************************************************************************
  * @文件名     ： main.c
  * @作者       ： Felix Wu
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： CANOpen主站Demo工程
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2018-11-14 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "bsp.h"
#include "app.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		256
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);


/************************************************
函数名称 ： SysInit
功    能 ： 系统初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： Felix Wu
*************************************************/
void SysInit(void)
{
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ： Felix Wu
*************************************************/
int main(void)
{
    /* 1、SYS初始化 */
    SysInit();

    /* 2、创建开始任务 */
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄

    /* 3. 开启任务调度 */
    vTaskStartScheduler();

    return 0;
}



/************************************************
函数名称 ： start_task
功    能 ： 开始任务
参    数 ： 无
返 回 值 ： void
作    者 ： Felix Wu
*************************************************/
void start_task(void *pvParameters)
{
    /* 进入临界区 */
    taskENTER_CRITICAL();

    /* 延时启动 */
//    vTaskDelay(1000);

    /* 创建其他任务 */
    AppTaskCreate();

    /* 删除开始任务 */
    vTaskDelete(StartTask_Handler);

    /* 退出临界区 */
    taskEXIT_CRITICAL();
}

/**** Copyright (C)2018 Felix Wu. All Rights Reserved **** END OF FILE ****/
