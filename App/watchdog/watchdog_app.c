/**
  ******************************************************************************
  * @�ļ���     �� watchdog_app.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��04��25��
  * @ժҪ       �� ���Ź�app����֤ϵͳ��ȫ�ɿ�����
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2019-03-01 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
#include "watchdog_app.h"
#include "bsp_usart.h"
/*
	ʹ�÷�����
	1. ��ʼ����ʱ������Լ���Ҫ�������ʱ��Ϊ200ms��ֱ���ں����и��ġ�
	2. ��������Ҫ��Task�У�ι��IWDG_Feed()
	ע�⣺
	1. ����ʹ�õĿ��Ź�Ϊ�������Ź������Ǵ��ڿ��Ź���֮�����봰�ڿ��Ź��Ĵ���
	2. �������Ź�ʹ�õ�ʱ�������õ�40kHz�Ķ���ʱ�ӣ���ʹ��ʱ�ӳ��ֹ��ϣ�Ҳ������reboot����
	3. ���ڿ��Ź�ʹ�õ�ʱ����APB1����˾�����Ը��ߣ�����һ����ʱ�ӳ��ֹ��ϣ�Ҳ��ͬʱ���ֹ���
	4. ���ڿ��Ź�һ����������쳣���⡣һ���������޷��رա�
*/

TaskHandle_t WATCHDOG_App_Task_Handler;

/* ��̬���� ------------------------------------------------------------------*/
void IWDG_Init(void);
void IWDG_Feed(void);
void WatchDog_App_Task(void *pvParameters);


/************************************************
�������� �� WatchDog_App_Init
��    �� �� LifterӦ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void WatchDog_App_Init(void)
{
    BaseType_t xReturn;
    IWDG_Init();
    xReturn = xTaskCreate(WatchDog_App_Task, "WatchDog_App_Task", WATCHDOG_STACK_SIZE, NULL, WATCHDOG_TASK_PRIORITY, &WATCHDOG_App_Task_Handler);
    if(pdPASS != xReturn)
    {
		printf("WatchDog_App created failed");
        return;                                      //������������ʧ��
    }
}


/************************************************
�������� �� WatchDog_App_Task
��    �� �� LIFTERӦ���������
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
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





/* ��ʼ�����Ź��� Init the IWDG */
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





/* ι���� Feed the IWDG */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

