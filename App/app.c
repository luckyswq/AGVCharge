/**
  ******************************************************************************
  * @文件名     ： app.c
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年11月14日
  * @摘要       ： 应用程序源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
   - 2019-03-02 V1.0.0: 初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "task.h"

#include "pccom_app.h"
#include "pccom_drv.h"
#include "watchdog_app.h"
#include "led_app.h"
#include "stepper_app.h"
#include "state_machine_app.h"

/* 静态申明 ------------------------------------------------------------------*/
static void APP_Init(void);
static void APP_Task(void *pvParameters);

//任务优先级
#define RUNTIMESTATS_TASK_PRIO	2
//任务堆栈大小
#define RUNTIMESTATS_STK_SIZE 	128
//任务句柄
TaskHandle_t RunTimeStats_App_Task_Handler;
//任务函数
void RunTimeStats_task(void *pvParameters);

char RunTimeInfo[400];		//保存任务运行时间信息


/************************************************
函数名称 ： AppTaskCreate
功    能 ： 创建任务
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void AppTaskCreate(void)
{
    BaseType_t xReturn;

    /* 创建应用程序(开始任务) */
    xReturn = xTaskCreate(APP_Task, "APP_Task", APP_STACK_SIZE, NULL, APP_TASK_PRIORITY, NULL);
    if(pdPASS != xReturn)
    {
        printf("APP_Task create failed");
        return;                                      //创建任务失败
    }
}

/************************************************
函数名称 ： APP_Init
功    能 ： 应用程序初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
static void APP_Init(void)
{
//    printf("APP_Init()\r\n");
    PCcom_App_Init();
    Led_App_Init();
    WatchDog_App_Init();
//    RunTimeStatsTaskCreate();
	Stepper_App_Init();
//	StatusManager_App_Init();
    USART_Cmd(PC_COM, ENABLE);
    USART_Cmd(STEPPER_COM, ENABLE);                                      //使能UART
}

/************************************************
函数名称 ： APP_Task
功    能 ： 应用程序(开始任务)
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
static void APP_Task(void *pvParameters)
{
//    taskENTER_CRITICAL();
    BSP_Init();                                    //底层驱动初始化
    APP_Init();                                    //应用程序初始化
//    taskEXIT_CRITICAL();

    for(;;)
    {
//		printf("APP_Task()");
        vTaskDelay(100);
    }
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{

    printf("任务：%s 发现栈溢出\r\n", pcTaskName);

}





//RunTimeStats任务
void RunTimeStats_task(void *pvParameters)
{
    while(1)
    {
        memset(RunTimeInfo,0,400);				//信息缓冲区清零
        vTaskList(RunTimeInfo);		//获取任务运行时间信息
        printf("Name\t\tState\tPrior\tStack\tNum\r\n");
        printf("%s\r\n",RunTimeInfo);

		memset(RunTimeInfo,0,400);				//信息缓冲区清零
        vTaskGetRunTimeStats(RunTimeInfo);		//获取任务运行时间信息
        printf("Name\t\tTime\t\tPercent\r\n");
        printf("%s\r\n",RunTimeInfo);
        vTaskDelay(2000);                           	//延时10ms，也就是1000个时钟节拍


    }
}

/************************************************
函数名称 ： RunTimeStatsTaskCreate
功    能 ： 创建任务
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void RunTimeStatsTaskCreate(void)
{
    BaseType_t xReturn;

    /* 创建应用程序(开始任务) */
    xReturn = xTaskCreate(RunTimeStats_task, "RunTimeStats_Task", RUNTIMESTATS_STACK_SIZE, NULL, RUNTIMESTATS_TASK_PRIORITY, &RunTimeStats_App_Task_Handler);
    if(pdPASS != xReturn)
    {
        printf("RunTimeStats_Task create failed");
        return;                                      //创建任务失败
    }
}


/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
