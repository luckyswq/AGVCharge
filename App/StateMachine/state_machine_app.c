#include "state_machine_app.h"


StatusManager_TypeDef StatusManager;
TaskHandle_t StatusManager_App_Task_Handler;
uint32_t g_status = 0;

/* --------------------------------------------------------------------*/
#define STATUSMANAGER_STACK_SIZE        128 
#define STATUSMANAGER_TASK_PRIORITY     3   

void StatusManager_App_Task(void *pvParameters);
static void StatusManager_Init(void);



/************************************************
函数名称 ： PCcom_App_Init
功    能 ： PCcom应用程序初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void StatusManager_App_Init(void)
{
    BaseType_t xReturn;

    StatusManager_Init();

    xReturn = xTaskCreate(StatusManager_App_Task, "StatusManager_App_Task", STATUSMANAGER_STACK_SIZE, NULL, STATUSMANAGER_TASK_PRIORITY, &StatusManager_App_Task_Handler);
    if(pdPASS != xReturn)
    {
       printf("StatusManager_App_Task create failed\r\n");
       return;                                      //创建接收任务失败
    }

}



/************************************************
函数名称 ： StatusManager_App_Task
功    能 ： StatusManager_App_Task应用任务程序
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void StatusManager_App_Task(void *pvParameters)
{
    for(;;)
    {
        switch (g_status)
        {
        case State0:
            /* code */
            g_status = State1;
            break;
        case State1:
            /* code */
            g_status = State0;
            break;
        
        default:
            break;
        }
//        printf("status= %d\r\n", g_status);
        vTaskDelay(1000);
    }

}



static void StatusManager_Init(void)
{
	g_status = Status_Init;
}



