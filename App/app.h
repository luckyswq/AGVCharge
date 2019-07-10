/**
  ******************************************************************************
  * @�ļ���     �� app.h
  * @����       �� Felix Wu
  * @�汾       �� V1.0.0
  * @����       �� 2018��11��14��
  * @ժҪ       �� Ӧ�ó���ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _APP_H
#define _APP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

/* �궨�� --------------------------------------------------------------------*/
#define APP_STACK_SIZE            128                      //��ʼ�����ջ��С
#define APP_TASK_PRIORITY         1                        //��ʼ�������ȼ�

/* �������� ------------------------------------------------------------------*/
void AppTaskCreate(void);
extern TaskHandle_t RunTimeStats_App_Task_Handler;
#define RUNTIMESTATS_STACK_SIZE        128                      //CANOPEN�����ջ��С
#define RUNTIMESTATS_TASK_PRIORITY     4                        //CANOPEN�������ȼ�


void RunTimeStatsTaskCreate(void);


#endif /* _APP_H */

/**** Copyright (C)2018 Felix Wu. All Rights Reserved **** END OF FILE ****/
