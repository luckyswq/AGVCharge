#ifndef __SERIAL_QUEUE_APP_H
#define __SERIAL_QUEUE_APP_H
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "bsp_usart.h"
#include "led_app.h"
#include "stepper_msg.h"
#include "stepper_drv.h"

extern TaskHandle_t recTaskHandle;
extern TaskHandle_t sendTaskHandle;

void data_queue_task_init(void);
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
//void RTUSend_Data(ArmMachine_TypeDef ArmMachineMsg);
void ArmMachineSend_Data(ArmMachine_TypeDef ArmMachineMsg);



#endif



