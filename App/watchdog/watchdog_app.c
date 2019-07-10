/**
  ******************************************************************************
  * @文件名     ： watchdog_app.c
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年04月25日
  * @摘要       ： 看门狗app，保证系统安全可靠运行
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2019-03-01 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
#include "watchdog_app.h"
#include "bsp_usart.h"
/*
	使用方法：
	1. 初始化，时间根据自己需要，这里的时间为200ms，直接在函数中更改。
	2. 在任意需要的Task中，喂狗IWDG_Feed()
	注意：
	1. 这里使用的看门狗为独立看门狗，不是窗口看门狗，之后会加入窗口看门狗的代码
	2. 独立看门狗使用的时钟是内置的40kHz的独立时钟，即使主时钟出现故障，也能正常reboot程序
	3. 窗口看门狗使用的时钟是APB1，因此精度相对更高，但是一旦主时钟出现故障，也会同时出现故障
	4. 窗口看门狗一般用于软件异常问题。一旦启动，无法关闭。
*/

TaskHandle_t WATCHDOG_App_Task_Handler;

/* 静态申明 ------------------------------------------------------------------*/
void IWDG_Init(void);
void IWDG_Feed(void);
void WatchDog_App_Task(void *pvParameters);


/************************************************
函数名称 ： WatchDog_App_Init
功    能 ： Lifter应用程序初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void WatchDog_App_Init(void)
{
    BaseType_t xReturn;
    IWDG_Init();
    xReturn = xTaskCreate(WatchDog_App_Task, "WatchDog_App_Task", WATCHDOG_STACK_SIZE, NULL, WATCHDOG_TASK_PRIORITY, &WATCHDOG_App_Task_Handler);
    if(pdPASS != xReturn)
    {
		printf("WatchDog_App created failed");
        return;                                      //创建接收任务失败
    }
}


/************************************************
函数名称 ： WatchDog_App_Task
功    能 ： LIFTER应用任务程序
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
static void WatchDog_App_Task(void *pvParameters)
{
    IWDG_Init();
    for(;;)
    {
        /* feed the dog every 10ms */
        vTaskDelay(100);
//        printf("Feed the dog");
        IWDG_Feed();
    }
}





/* 初始化看门狗： Init the IWDG */
void IWDG_Init(void)
{
    /* Enable the IWDG write */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /*
     * 40K/256 	= 	156Hz(6.4ms)
     * 40K/8 	=	5KHz(0.2ms)
     */
    IWDG_SetPrescaler(IWDG_Prescaler_128);  // should be 8

    /* 1000x0.2 = 200ms, the param value should be < 0xfff */
    IWDG_SetReload(1000);

    IWDG_ReloadCounter();

    IWDG_Enable();
}





/* 喂狗： Feed the IWDG */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

