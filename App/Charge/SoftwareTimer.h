#ifndef __SOFTWARETIMER_H
#define __SOFTWARETIMER_H
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
//#include "nvidiacom_drv.h"
//#include "global_var.h"
#include "timers.h"
#define SAFETYMANAGER_STACK_SIZE 128
#define SAFETYMANAGER_TASK_PRIORITY 3
#define SAFETY_TIME_GAP_BETWEEN_CMDS 10 			// ms

extern TaskHandle_t Software_Timer_Task_Handler;
extern TimerHandle_t SoftwareTimer_Handle;			//周期定时器句柄

void SoftwareTimer_Init(void);
static void SoftTimer_Task(void *pvParameters);
void SoftwareCallback(TimerHandle_t xTimer);
#endif
