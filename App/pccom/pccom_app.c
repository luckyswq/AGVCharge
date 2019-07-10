/**
  ******************************************************************************
  * @文件名     ： pccom.c
  * @作者       ： FelixWu
  * @版本       ： V1.0.0
  * @日期       ： 2019年03月01日
  * @摘要       ： arm board communication with nvidia board
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2019-03-01 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
#include "pccom_app.h"
#include "math.h"
#include "serial_queue_app.h"

/* 静态申明 ------------------------------------------------------------------*/
static void PCcom_Rec_App_Task(void *pvParameters);
TaskHandle_t PCcom_Rec_App_Task_Handler;

//static void PCcom_Send_App_Task(void *pvParameters);
//TaskHandle_t PCcom_Send_App_Task_Handler;


/************************************************
函数名称 ： PCcom_App_Init
功    能 ： PCcom应用程序初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
void PCcom_App_Init(void)
{
    BaseType_t xReturn;

    PCcom_Driver_Init();

    xReturn = xTaskCreate(PCcom_Rec_App_Task, "PCcom_Rec_App_Task", PCCOM_STACK_SIZE, NULL, PCCOM_TASK_PRIORITY, &PCcom_Rec_App_Task_Handler);
    if(pdPASS != xReturn)
    {
        printf("PCcom_Rec_App_Task create failed\r\n");
        return;                                      //创建接收任务失败
    }
//    xReturn = xTaskCreate(PCcom_Send_App_Task, "PCcom_Send_App_Task", PCCOM_STACK_SIZE, NULL, PCCOM_TASK_PRIORITY, &PCcom_Send_App_Task_Handler);
//    if(pdPASS != xReturn)
//    {
//        return;                                      //创建Send任务失败
//    }
}


/************************************************
函数名称 ： PCcom_Rec_App_Task
功    能 ： PCcom应用任务程序
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： FelixWu
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
			switch(NotifyValue)// 根据串口收到的不同的指令代码，进行不同的操作。
			{
				/*  Infos about arm machine
					左右移动（伸缩）直径27mm，0-200mm行程
					旋转 齿轮1:4减速比 0-180度角度范围
					上下 0-50mm行程，1圈1mm  */
                case 0x00:
                    // (x, y)
                    target_cartesian.x = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[3] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[4] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[5] << 8) + PC_COM_RX_BUF_BACKUP[6]) / 1000.0; // 单位：um/1000 ==> mm
                    target_cartesian.y = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[7] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[8] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[9] << 8) + PC_COM_RX_BUF_BACKUP[10]) / 1000.0; // 单位：um/1000 ==> mm
                    printf("x = %.3fmm\ty = %.3fmm\r\n", target_cartesian.x, target_cartesian.y); //
					
					arm_out_len = sqrt(target_cartesian.y * target_cartesian.y + target_cartesian.x * target_cartesian.x) - 100; // mm  -100 means to get the arm length head out of the machine
					ArmMachineMsg.armMotor.targetPosition = arm_out_len / (3.141593 * 27) * 1000; // steps (absolute)
					ArmMachineMsg.armMotor.targetPosition = ArmMachineMsg.armMotor.targetPosition * (-1); // 方向相反

					base_rotate_ang = atan2(target_cartesian.y , target_cartesian.x) * 180.0 / 3.141593; // 0~180 deg
					ArmMachineMsg.baseMotor.targetPosition = base_rotate_ang * 4 / 360.0 * 1000; // steps (absolute)
				
					ArmMachineMsg.updownMotor.targetPosition = 40000;// 0 mm = 0 step // TOP


					printf("arm = %d steps\tbase = %d steps\r\n", ArmMachineMsg.armMotor.targetPosition, ArmMachineMsg.baseMotor.targetPosition); // 
					xTaskNotify(sendTaskHandle, 1, (eNotifyAction) eSetValueWithOverwrite);//发送任务通知
				
                    break;
				
                case 0x01:
                    // (theta, distance)
                    target_polar.theta = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[3] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[4] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[5] << 8) + PC_COM_RX_BUF_BACKUP[6]) / 1000.0;
                    target_polar.distance = (int32_t)((uint32_t)(PC_COM_RX_BUF_BACKUP[7] << 24) + (uint32_t)(PC_COM_RX_BUF_BACKUP[8] << 16) + (uint32_t)(PC_COM_RX_BUF_BACKUP[9] << 8) + PC_COM_RX_BUF_BACKUP[10]) / 1000.0;
                    printf("d = %.3fmm\ttheta = %.3fdeg\r\n", target_polar.distance, target_polar.theta);
				
					arm_out_len = target_polar.distance - 100; // mm   -100 means to get the arm length head out of the machine
					ArmMachineMsg.armMotor.targetPosition = arm_out_len / (3.141593 * 27) * 1000; // steps (absolute)
					ArmMachineMsg.armMotor.targetPosition = ArmMachineMsg.armMotor.targetPosition * (-1); // 方向相反
				
					base_rotate_ang =  target_polar.theta; // 0~180 deg
					ArmMachineMsg.baseMotor.targetPosition = base_rotate_ang * 4 / 360.0 * 1000; // steps (absolute)

					ArmMachineMsg.updownMotor.targetPosition = 40000;// 0.1 mm = 0 step // TOP

					printf("arm = %d steps\tbase = %d steps\r\n", ArmMachineMsg.armMotor.targetPosition, ArmMachineMsg.baseMotor.targetPosition); // 
					xTaskNotify(sendTaskHandle, 1, (eNotifyAction) eSetValueWithOverwrite);//发送任务通知
				
                    break;
				
                case 0x02:
                    // (lift up and drop down)
                    if(PC_COM_RX_BUF_BACKUP[3] == 0x0F)
                    {
                        // lift up
						ArmMachineMsg.updownMotor.targetPosition = 40000;// 40 mm = 40000 step
						xTaskNotify(sendTaskHandle, 2, (eNotifyAction) eSetValueWithOverwrite);//发送任务通知
                    }
                    else
                    {
                        // drop down
						ArmMachineMsg.updownMotor.targetPosition = 100;// 0.1 mm = 100 step
						xTaskNotify(sendTaskHandle, 3, (eNotifyAction) eSetValueWithOverwrite);//发送任务通知
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
        /* 应用代码 */
    }
}

/************************************************
函数名称 ： PCcom_Send_App_Task
功    能 ： PCcom应用任务程序
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： FelixWu
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
函数名称 ： PC_COM_IRQHandler
功    能 ： USARTx中断(通讯)
参    数 ： 无
返 回 值 ： 无
作    者 ： FelixWu
*************************************************/
//void PC_COM_IRQHandler(void)
//{
//    BaseType_t xHigherPriorityTaskWoken = pdTRUE;

//    if(USART_GetITStatus(PC_COM, USART_IT_RXNE) != RESET)
//    {
//        /* START --- 添加在这里的内容，是为了保证清除了标志 */
//        USART_ClearITPendingBit(PC_COM, USART_IT_RXNE);
//        /* END ---- 添加在这里的内容，是为了保证清除了标志 */

//        PC_COM_RX_BUF[PC_COM_RX_STA++] = USART_ReceiveData(PC_COM);
//    }
//    else if(USART_GetITStatus(PC_COM, USART_IT_IDLE) != RESET)	// 如果接收到完整的一帧数据
//    {
//        USART_ReceiveData(PC_COM);
//        PC_COM_RX_STA=0;
//        USART_ClearITPendingBit(PC_COM,USART_IT_IDLE);

//        /* Notify battery更新task */
//        for(int i = 0 ; i < PC_COM_REC_LEN; i ++)
//            PC_COM_RX_BUF_BACKUP[i] = PC_COM_RX_BUF[i];
//		
//		if(PCcom_Rec_App_Task_Handler!=NULL) {
//			xTaskNotifyFromISR(PCcom_Rec_App_Task_Handler, PC_COM_RX_BUF[2], (eNotifyAction) eSetValueWithOverwrite,&xHigherPriorityTaskWoken);//发送任务通知
//			portYIELD_FROM_ISR(PCcom_Rec_App_Task_Handler);//如果需要的话进行一次任务切换
//		}

//    }

//    /* START --- 添加在这里的内容，是为了保证清除了标志 */
//    //发的地方清标志
//    if(USART_GetFlagStatus(PC_COM,USART_FLAG_ORE) == SET)
//    {
//        USART_ClearFlag(PC_COM,USART_FLAG_ORE);
//        USART_ReceiveData(PC_COM);
//    }
//    /* END ---- 添加在这里的内容，是为了保证清除了标志 */
//}


/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/
