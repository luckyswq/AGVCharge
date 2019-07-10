/**
  ******************************************************************************
  * @�ļ���     �� nvidia_drv.h
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��01��
  * @ժҪ       �� Nvidia com with armӦ�ó���ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _PCCOM_APP_H
#define _PCCOM_APP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "bsp_usart.h"
#include "stepper_msg.h"
#include "pccom_drv.h"

/* �궨�� --------------------------------------------------------------------*/
#define PCCOM_STACK_SIZE        128                      //CANOPEN�����ջ��С
#define PCCOM_TASK_PRIORITY     3                        //CANOPEN�������ȼ�


extern TaskHandle_t PCcom_App_Task_Handler;


/* �������� ------------------------------------------------------------------*/
void PCcom_App_Init(void);

void PC_COM_IRQHandler(void);


#endif /* _PCCOM_APP_H */

/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
