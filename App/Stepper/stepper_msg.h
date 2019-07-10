#ifndef __STEPPER_MSG_H
#define __STEPPER_MSG_H
#include <stdint.h>
//typedef int32_t
/*
	Infos about arm machine
	
	左右移动（伸缩）直径27mm，0-200mm行程
	旋转 齿轮1:4减速比 0-180度角度范围
	上下 0-50mm行程，1圈1mm

*/



typedef struct {
    // 控制指令信息
    // 起动/变速斜率[kHz/s]		停止斜率[kHz/s]	最大速度[Hz]  目标位置[step]
    int32_t startSlope;					// eg: 500
    int32_t stopSlope;					// eg: 500
    int32_t topSpeed;						// eg: 2000
    int32_t targetPosition; 		// range: [-2147483648, 2147483647]
    char mode;									// value: {'r', 'a},	r-->relative, a-->absolute

    // 警报代码
    uint16_t alarmCode;					// alarm code of the stepper

    // 其他信息
    // 减速比		步距-->角度（底部旋转电机）		步距-->距离（升/降电机 和 伸出/缩回电机）		从站地址
    uint8_t address;
    uint16_t stepPerRound;			// eg: 1000
    uint16_t reductionRatio;
    float step2angular;					// unit: deg
    float step2distance;				// unit: mm

} StepperMsg_TypeDef;

typedef struct {
    // 底部旋转电机			升/降电机		伸出/缩回电机
    StepperMsg_TypeDef baseMotor;
    StepperMsg_TypeDef updownMotor;
    StepperMsg_TypeDef armMotor;
} ArmMachine_TypeDef;


typedef struct {
	double theta;
	double distance;
} TargetPolar_TypeDef;

typedef struct {
	double x;
	double y;
} TargetCartesian_TypeDef;



extern ArmMachine_TypeDef ArmMachineMsg;
extern TargetPolar_TypeDef target_polar;
extern TargetCartesian_TypeDef target_cartesian;


#endif

