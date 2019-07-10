/**
  ******************************************************************************
  * @�ļ���     �� pccom.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��01��
  * @ժҪ       �� arm board communication with nvidia board
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2019-03-01 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
#include "pccom_app.h"
#include "math.h"
#include "serial_queue_app.h"

/* ��̬���� ------------------------------------------------------------------*/
static void PCcom_Rec_App_Task(void *pvParameters);
TaskHandle_t PCcom_Rec_App_Task_Handler;

//static void PCcom_Send_App_Task(void *pvParameters);
//TaskHandle_t PCcom_Send_App_Task_Handler;


/************************************************
�������� �� PCcom_App_Init
��    �� �� PCcomӦ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void PCcom_App_Init(void)
{
    BaseType_t xReturn;

    PCcom_Driver_Init();

    xReturn = xTaskCreate(PCcom_Rec_App_Task, "PCcom_Rec_App_Task", PCCOM_STACK_SIZE, NULL, PCCOM_TASK_PRIORITY, &PCcom_Rec_App_Task_Handler);
    if(pdPASS != xReturn)
    {
        printf("PCcom_Rec_App_Task create failed\r\n");
        return;                                      //������������ʧ��
    }
//    xReturn = xTaskCreate(PCcom_Send_App_Task, "PCcom_Send_App_Task", PCCOM_STACK_SIZE, NULL, PCCOM_TASK_PRIORITY, &PCcom_Send_App_Task_Handler);
//    if(pdPASS != xReturn)
//    {
//        return;                                      //����Send����ʧ��
//    }
}


/************************************************
�������� �� PCcom_Rec_App_Task
��    �� �� PCcomӦ���������
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
static void PCcom_Rec_App_Task(void *pvParameters)
{
	u32 NotifyValue = 0;
	BaseType_t err;
	double arm_out_len = 0.0;
	double base_rotate_ang = 0.0;
    for(;;)
    {
		err = xTaskNotifyWait(0x00, 0xffffffff, &NotifyValue, portMAX_DELAY);
		if(err == pdPASS)
		{
			switch(NotifyValue)// ���ݴ����յ��Ĳ�ͬ��ָ����룬���в�ͬ�Ĳ�����
			{
				/*  Infos about arm machine
					�����ƶ���������ֱ��27mm��0-200mm�г�
					��ת ����1:4���ٱ� 0-180�ȽǶȷ�Χ
					���� 0-50mm�г̣�1Ȧ1mm  */
                case 0x00:
                    // (x, y)
                    target_cartesian.x = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[3] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[4] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[5] << 8) + PC_COM_RX_BUF_BACKUP[6]) / 1000.0; // ��λ��um/1000 ==> mm
                    target_cartesian.y = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[7] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[8] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[9] << 8) + PC_COM_RX_BUF_BACKUP[10]) / 1000.0; // ��λ��um/1000 ==> mm
                    printf("x = %.3fmm\ty = %.3fmm\r\n", target_cartesian.x, target_cartesian.y); //
					
					arm_out_len = sqrt(target_cartesian.y * target_cartesian.y + target_cartesian.x * target_cartesian.x) - 100; // mm  -100 means to get the arm length head out of the machine
					ArmMachineMsg.armMotor.targetPosition = arm_out_len / (3.141593 * 27) * 1000; // steps (absolute)
					ArmMachineMsg.armMotor.targetPosition = ArmMachineMsg.armMotor.targetPosition * (-1); // �����෴

					base_rotate_ang = atan2(target_cartesian.y , target_cartesian.x) * 180.0 / 3.141593; // 0~180 deg
					ArmMachineMsg.baseMotor.targetPosition = base_rotate_ang * 4 / 360.0 * 1000; // steps (absolute)
				
					ArmMachineMsg.updownMotor.targetPosition = 40000;// 0 mm = 0 step // TOP


					printf("arm = %d steps\tbase = %d steps\r\n", ArmMachineMsg.armMotor.targetPosition, ArmMachineMsg.baseMotor.targetPosition); // 
					xTaskNotify(sendTaskHandle, 1, (eNotifyAction) eSetValueWithOverwrite);//��������֪ͨ
				
                    break;
				
                case 0x01:
                    // (theta, distance)
                    target_polar.theta = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[3] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[4] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[5] << 8) + PC_COM_RX_BUF_BACKUP[6]) / 1000.0;
                    target_polar.distance = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[7] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[8] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[9] << 8) + PC_COM_RX_BUF_BACKUP[10]) / 1000.0;
                    printf("d = %.3fmm\ttheta = %.3fdeg\r\n", target_polar.distance, target_polar.theta);
				
					arm_out_len = target_polar.distance - 100; // mm   -100 means to get the arm length head out of the machine
					ArmMachineMsg.armMotor.targetPosition = arm_out_len / (3.141593 * 27) * 1000; // steps (absolute)
					ArmMachineMsg.armMotor.targetPosition = ArmMachineMsg.armMotor.targetPosition * (-1); // �����෴
				
					base_rotate_ang =  target_polar.theta; // 0~180 deg
					ArmMachineMsg.baseMotor.targetPosition = base_rotate_ang * 4 / 360.0 * 1000; // steps (absolute)

					ArmMachineMsg.updownMotor.targetPosition = 40000;// 0.1 mm = 0 step // TOP

					printf("arm = %d steps\tbase = %d steps\r\n", ArmMachineMsg.armMotor.targetPosition, ArmMachineMsg.baseMotor.targetPosition); // 
					xTaskNotify(sendTaskHandle, 1, (eNotifyAction) eSetValueWithOverwrite);//��������֪ͨ
				
                    break;
				
                case 0x02:
                    // (lift up and drop down)
                    if(PC_COM_RX_BUF_BACKUP[3] == 0x0F)
                    {
                        // lift up
						ArmMachineMsg.updownMotor.targetPosition = 40000;// 40 mm = 40000 step
						xTaskNotify(sendTaskHandle, 2, (eNotifyAction) eSetValueWithOverwrite);//��������֪ͨ
                    }
                    else
                    {
                        // drop down
						ArmMachineMsg.updownMotor.targetPosition = 100;// 0.1 mm = 100 step
						xTaskNotify(sendTaskHandle, 3, (eNotifyAction) eSetValueWithOverwrite);//��������֪ͨ
                    }
                    break;
                case 0x03:
                    // (activate and deactivate sucker)
                    if(PC_COM_RX_BUF_BACKUP[3] == 0x01)
                    {
                        // activate
                    }
                    else
                    {
                        // deactivate
                    }
                    
                    printf("0x03");
                    break;
                default:
                    break;
			}		
		}
        vTaskDelay(1);
        /* Ӧ�ô��� */
    }
}

/************************************************
�������� �� PCcom_Send_App_Task
��    �� �� PCcomӦ���������
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
//static void PCcom_Send_App_Task(void *pvParameters)
//{
//	u32 NotifyValue = 0;
//	BaseType_t err;
//    for(;;)
//    {
//		err = xTaskNotifyWait(0x00, 0xffffffff, &NotifyValue, portMAX_DELAY);
//		if(err == pdPASS)
//		{
//            printf("update the status");
//            // send the status to PC
//        }
//    }

//}
/************************************************
�������� �� PC_COM_IRQHandler
��    �� �� USARTx�ж�(ͨѶ)
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
//void PC_COM_IRQHandler(void)
//{
//    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

//    if(USART_GetITStatus(PC_COM, USART_IT_RXNE) != RESET)
//    {
//        /* START --- �������������ݣ���Ϊ�˱�֤����˱�־ */
//        USART_ClearITPendingBit(PC_COM, USART_IT_RXNE);
//        /* END ---- �������������ݣ���Ϊ�˱�֤����˱�־ */

//        PC_COM_RX_BUF[PC_COM_RX_STA++] = USART_ReceiveData(PC_COM);
//    }
//    else if(USART_GetITStatus(PC_COM, USART_IT_IDLE) != RESET)	// ������յ�������һ֡����
//    {
//        USART_ReceiveData(PC_COM);
//        PC_COM_RX_STA=0;
//        USART_ClearITPendingBit(PC_COM,USART_IT_IDLE);

//        /* Notify battery����task */
//        for(int i = 0 ; i < PC_COM_REC_LEN; i ++)
//            PC_COM_RX_BUF_BACKUP[i] = PC_COM_RX_BUF[i];
//		
//		if(PCcom_Rec_App_Task_Handler!=NULL) {
//			xTaskNotifyFromISR(PCcom_Rec_App_Task_Handler, PC_COM_RX_BUF[2], (eNotifyAction) eSetValueWithOverwrite,&xHigherPriorityTaskWoken);//��������֪ͨ
//			portYIELD_FROM_ISR(PCcom_Rec_App_Task_Handler);//�����Ҫ�Ļ�����һ�������л�
//		}

//    }

//    /* START --- �������������ݣ���Ϊ�˱�֤����˱�־ */
//    //���ĵط����־
//    if(USART_GetFlagStatus(PC_COM,USART_FLAG_ORE) == SET)
//    {
//        USART_ClearFlag(PC_COM,USART_FLAG_ORE);
//        USART_ReceiveData(PC_COM);
//    }
//    /* END ---- �������������ݣ���Ϊ�˱�֤����˱�־ */
//}


/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
