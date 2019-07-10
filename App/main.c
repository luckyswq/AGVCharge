/**
  ******************************************************************************
  * @�ļ���     �� main.c
  * @����       �� Felix Wu
  * @�汾       �� V1.0.0
  * @����       �� 2018��11��14��
  * @ժҪ       �� CANOpen��վDemo����
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2018-11-14 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "bsp.h"
#include "app.h"

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С
#define START_STK_SIZE 		256
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);


/************************************************
�������� �� SysInit
��    �� �� ϵͳ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� Felix Wu
*************************************************/
void SysInit(void)
{
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

/************************************************
�������� �� main
��    �� �� ���������
��    �� �� ��
�� �� ֵ �� int
��    �� �� Felix Wu
*************************************************/
int main(void)
{
    /* 1��SYS��ʼ�� */
    SysInit();

    /* 2��������ʼ���� */
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������

    /* 3. ����������� */
    vTaskStartScheduler();

    return 0;
}



/************************************************
�������� �� start_task
��    �� �� ��ʼ����
��    �� �� ��
�� �� ֵ �� void
��    �� �� Felix Wu
*************************************************/
void start_task(void *pvParameters)
{
    /* �����ٽ��� */
    taskENTER_CRITICAL();

    /* ��ʱ���� */
//    vTaskDelay(1000);

    /* ������������ */
    AppTaskCreate();

    /* ɾ����ʼ���� */
    vTaskDelete(StartTask_Handler);

    /* �˳��ٽ��� */
    taskEXIT_CRITICAL();
}

/**** Copyright (C)2018 Felix Wu. All Rights Reserved **** END OF FILE ****/
