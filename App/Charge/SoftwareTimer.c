/**
  ******************************************************************************
  * @�ļ���     �� safetymanager_app.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��20��
  * @ժҪ       �� safetymanager_app
				   ��ȫ�����������ʱ��û�н��յ����ڵ��ٶȿ���ָ��򽫷���ָֹͣ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2019-03-01 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
//#include "safetymanager_app.h"
//#include "canopen_drv.h"
//#include "canopen_app.h"
#include "SoftwareTimer.h"
#include "bsp.h"
#include "modbus.h"
#include "led_app.h"

TaskHandle_t Software_Timer_Task_Handler;
TimerHandle_t SoftwareTimer_Handle;			//���ڶ�ʱ�����




/************************************************
�������� �� SafetyManager_App_Init
��    �� �� SafetyManagerӦ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void SoftwareTimer_Init(void)
{
  BaseType_t xReturn;

  xReturn = xTaskCreate(SoftTimer_Task, "SoftTimer_Task", SAFETYMANAGER_STACK_SIZE, NULL, SAFETYMANAGER_TASK_PRIORITY, &Software_Timer_Task_Handler);
  if(pdPASS != xReturn)
  {
    return;                                      //������������ʧ��
  }
}

/************************************************
�������� �� StatusManager_App_Task
��    �� �� StatusManager_App_Task
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
static void SoftTimer_Task(void *pvParameters)
{  
//	CanRxMsg RxMsg;
    //�����������ڶ�ʱ��
    SoftwareTimer_Handle=xTimerCreate((const char*		)"SoftwareTimer",
																			(TickType_t			)SAFETY_TIME_GAP_BETWEEN_CMDS,
																			(UBaseType_t		)pdTRUE,
																			(void*					)1,
																			(TimerCallbackFunction_t)SoftwareCallback); //���ڶ�ʱ��������10ms(1000��ʱ�ӽ���)������ģʽ

	if(SoftwareTimer_Handle!=NULL)
	{
		// �����ʱ�������ɹ���
		xTimerStart(SoftwareTimer_Handle,0);	//�������ڶ�ʱ��
	}
									
	for(;;)
	{
		vTaskDelay(1000);
		// ��ʱ֮�󣬲���Ҫִ�б�task��ֻ��Ҫ�ڶ�ʱ���Ļص������д���
	}
}

/************************************************
�������� �� AutoReloadCallback
��    �� �� ���ڶ�ʱ���Ļص�����
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void SoftwareCallback(TimerHandle_t xTimer)
{
	LED2_TOGGLE();   //LED���
		if(HMITimerOut<0xFFFE)HMITimerOut++;
		else HMITimerOut=0; 
		if(HMITimerOut>100)  //HMI��ʱ����
		{
			CommIndex=0;
			HMITimerOut=0;
		}
}




/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
