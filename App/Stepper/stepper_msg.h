#ifndef __STEPPER_MSG_H
#define __STEPPER_MSG_H
#include <stdint.h>
//typedef int32_t
/*
	Infos about arm machine
	
	�����ƶ���������ֱ��27mm��0-200mm�г�
	��ת ����1:4���ٱ� 0-180�ȽǶȷ�Χ
	���� 0-50mm�г̣�1Ȧ1mm

*/



typedef struct {
    // ����ָ����Ϣ
    // ��/����б��[kHz/s]		ֹͣб��[kHz/s]	����ٶ�[Hz]  Ŀ��λ��[step]
    int32_t startSlope;					// eg: 500
    int32_t stopSlope;					// eg: 500
    int32_t topSpeed;						// eg: 2000
    int32_t targetPosition; 		// range: [-2147483648, 2147483647]
    char mode;									// value: {'r', 'a},	r-->relative, a-->absolute

    // ��������
    uint16_t alarmCode;					// alarm code of the stepper

    // ������Ϣ
    // ���ٱ�		����-->�Ƕȣ��ײ���ת�����		����-->���루��/����� �� ���/���ص����		��վ��ַ
    uint8_t address;
    uint16_t stepPerRound;			// eg: 1000
    uint16_t reductionRatio;
    float step2angular;					// unit: deg
    float step2distance;				// unit: mm

} StepperMsg_TypeDef;

typedef struct {
    // �ײ���ת���			��/�����		���/���ص��
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

