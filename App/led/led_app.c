/**
  ******************************************************************************
  * @�ļ���     �� button_app.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��07��
  * @ժҪ       �� button_app
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2019-03-01 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
#include "led_app.h"
#include "bsp_usart.h"
TaskHandle_t Led_App_Task_Handler;


/* ��̬���� ------------------------------------------------------------------*/
static void Led_App_Task(void *pvParameters);
void Led_IO_Config_Init(void);


/************************************************
�������� �� Led_App_Init
��    �� �� LedӦ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void Led_App_Init(void)
{
    BaseType_t xReturn;
    Led_IO_Config_Init();
    xReturn = xTaskCreate(Led_App_Task, "Led_App_Task", LED_STACK_SIZE, NULL, LED_TASK_PRIORITY, &Led_App_Task_Handler);
    if(pdPASS != xReturn)
    {
        printf("Led_App create failed");
        return;                                      //������������ʧ��
    }
}

/************************************************
�������� �� Led_App_Task
��    �� �� LedӦ���������
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
static void Led_App_Task(void *pvParameters)
{
    for(;;)
    {
        LED1_TOGGLE();
//        LED2_TOGGLE();
//		printf("Led_App_Task()");
        vTaskDelay(50);
    }
}

/************************************************
�������� �� Led_IO_Config_Init
��    �� �� Led IO��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void Led_IO_Config_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

}


/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/