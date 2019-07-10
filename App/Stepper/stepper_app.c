#include "stepper_app.h"
#include "serial_queue_app.h"


TaskHandle_t stepperTaskHandle;
void Stepper_App_Task(void * argument);



void Stepper_App_Init(void)
{
//	printf("Init --> StepperInit()\r\n");
    BaseType_t xReturn;
	UART_Stepper_Configuration();
    data_queue_task_init();

    /* 创建任务 */
    xReturn = xTaskCreate(Stepper_App_Task, "Stepper_App_Task", 128, NULL, 3,	&stepperTaskHandle);
    if(pdPASS != xReturn)
    {
        printf("StepperTask create failed\r\n");
        return;                                      //创建接收任务失败
    }
}

void Stepper_App_Task(void * argument)
{
    msg_init(&ArmMachineMsg);
//	convertMsgIntoRTU(ArmMachineMsg);
    for(;;)
    {
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
//		convertMsgIntoRTU(ArmMachineMsg);
//        stepper_test();
//		printf("Task --> StepperTask()\r\n");
        vTaskDelay(3000);
    }
}



void msg_init(ArmMachine_TypeDef *ArmMachineMsg)
{
    StepperMsg_TypeDef baseMotor;
    StepperMsg_TypeDef updownMotor;
    StepperMsg_TypeDef armMotor;

    // baseMotor
    baseMotor.startSlope = 100;		// 1.5kHz
    baseMotor.stopSlope = 100;
    baseMotor.targetPosition = 0;
    baseMotor.topSpeed = 500;
    baseMotor.mode = 'a';
    baseMotor.alarmCode = 0x00;

    baseMotor.address = 0x03;
    baseMotor.reductionRatio = 4;		// gearRatio = 1 : 4
    baseMotor.stepPerRound = 1000;
    baseMotor.step2angular = 360.0 / baseMotor.stepPerRound / baseMotor.reductionRatio;		//
    baseMotor.step2distance = 0.0;

    // updownMotor
    updownMotor.startSlope = 10000;
    updownMotor.stopSlope = 10000;
    updownMotor.targetPosition = 200;
    updownMotor.topSpeed = 50000;
    updownMotor.mode = 'a';
    updownMotor.alarmCode = 0x00;

    updownMotor.address = 0x02;
    updownMotor.reductionRatio = 1;		// NA
    updownMotor.stepPerRound = 1000;
    updownMotor.step2angular = 0.0;
    updownMotor.step2distance = 1.0 / updownMotor.stepPerRound;		// 导程1mm，步进电机1圈对应1mm距离变化

    // armMotor
    armMotor.startSlope = 100;
    armMotor.stopSlope = 100;
    armMotor.targetPosition = 0;
    armMotor.topSpeed = 500;
    armMotor.mode = 'a';
    armMotor.alarmCode = 0x00;

    armMotor.address = 0x01;
    armMotor.reductionRatio = 1;		// NA
    armMotor.stepPerRound = 1000;
    armMotor.step2angular = 0.0;
    armMotor.step2distance = 3.1415927 * 27 / armMotor.stepPerRound;		// 齿轮直径27mm

    ArmMachineMsg->armMotor = armMotor;
    ArmMachineMsg->baseMotor = baseMotor;
    ArmMachineMsg->updownMotor = updownMotor;
}


