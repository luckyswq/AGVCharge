/**
  ******************************************************************************
  * @�ļ���     �� app.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��11��14��
  * @ժҪ       �� Ӧ�ó���Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
   - 2019-03-02 V1.0.0: ��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
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

/* ��̬���� ------------------------------------------------------------------*/
static void APP_Init(void);
static void APP_Task(void *pvParameters);

//�������ȼ�
#define RUNTIMESTATS_TASK_PRIO	2
//�����ջ��С
#define RUNTIMESTATS_STK_SIZE 	128
//������
TaskHandle_t RunTimeStats_App_Task_Handler;
//������
void RunTimeStats_task(void *pvParameters);

char RunTimeInfo[400];		//������������ʱ����Ϣ


/************************************************
�������� �� AppTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void AppTaskCreate(void)
{
    BaseType_t xReturn;

    /* ����Ӧ�ó���(��ʼ����) */
    xReturn = xTaskCreate(APP_Task, "APP_Task", APP_STACK_SIZE, NULL, APP_TASK_PRIORITY, NULL);
    if(pdPASS != xReturn)
    {
        printf("APP_Task create failed");
        return;                                      //��������ʧ��
    }
}

/************************************************
�������� �� APP_Init
��    �� �� Ӧ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
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
    USART_Cmd(STEPPER_COM, ENABLE);                                      //ʹ��UART
}

/************************************************
�������� �� APP_Task
��    �� �� Ӧ�ó���(��ʼ����)
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
static void APP_Task(void *pvParameters)
{
//    taskENTER_CRITICAL();
    BSP_Init();                                    //�ײ�������ʼ��
    APP_Init();                                    //Ӧ�ó����ʼ��
//    taskEXIT_CRITICAL();

    for(;;)
    {
//		printf("APP_Task()");
        vTaskDelay(100);
    }
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{

    printf("����%s ����ջ���\r\n", pcTaskName);

}





//RunTimeStats����
void RunTimeStats_task(void *pvParameters)
{
    while(1)
    {
        memset(RunTimeInfo,0,400);				//��Ϣ����������
        vTaskList(RunTimeInfo);		//��ȡ��������ʱ����Ϣ
        printf("Name\t\tState\tPrior\tStack\tNum\r\n");
        printf("%s\r\n",RunTimeInfo);

		memset(RunTimeInfo,0,400);				//��Ϣ����������
        vTaskGetRunTimeStats(RunTimeInfo);		//��ȡ��������ʱ����Ϣ
        printf("Name\t\tTime\t\tPercent\r\n");
        printf("%s\r\n",RunTimeInfo);
        vTaskDelay(2000);                           	//��ʱ10ms��Ҳ����1000��ʱ�ӽ���


    }
}

/************************************************
�������� �� RunTimeStatsTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void RunTimeStatsTaskCreate(void)
{
    BaseType_t xReturn;

    /* ����Ӧ�ó���(��ʼ����) */
    xReturn = xTaskCreate(RunTimeStats_task, "RunTimeStats_Task", RUNTIMESTATS_STACK_SIZE, NULL, RUNTIMESTATS_TASK_PRIORITY, &RunTimeStats_App_Task_Handler);
    if(pdPASS != xReturn)
    {
        printf("RunTimeStats_Task create failed");
        return;                                      //��������ʧ��
    }
}


/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
