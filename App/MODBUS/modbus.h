#ifndef __MODBUS_H
#define __MODBUS_H

//#include "stm32f10x_lib.h"
/* These macro for Mudbus function code */
#include "FreeRTOS.h"
#include "semphr.h"	 
#include "task.h"

#include "HMI_dev.h"

#define ReadCoilSta				0x01 // 0X read coil function
#define ReadBitSta				0x02
#define ForceSingleCoil 	0x05 // 0X force single coil function
#define ReadHoldReg 			0x03 // 4X read input register
#define PresetMulReg 			0x06 // 4X write multi register
#define PresetMulReg32  	0x10 
/* These macro describe index of communiction message from start to end */
#define CommIndexSlaveID		0 // macro for slave ID
#define CommIndexFunction 	1 // macro for function code
#define CommIndexStartAdrHi 2 // macro for data hight adr
#define CommIndexStartAdrLo 3 // macro for data low adr
#define CommIndexNoPointHi 	4 // macro for hight quantity datas
#define CommIndexNoPointLo 	5 // macro for low quantity datas
#define	CommIndex2NoPointHi	6
#define	CommIndex2NoPointLo 7


/* These macro describe recieve message lenght of different function code */
/* But not include data lenght ,the data is master write to slave */
/* because we don't know the data lenght prior, it depend on ComBuf[ComIndexNoPoint] value */
#define ReadCoilStaIndexEnd 			7           // 0~7,0x01,0X
#define ReadBitStaIndexEnd 				7           // 0~7,0x01,0X
#define ForceSingleCoilIndexEnd 	    7           // 0~7,0x05,0X
#define ReadHoldRegIndexEnd 			7           // 0~7,0x03,4X
#define PresetMulRegIndexEnd 			7           // 0~8,0x10,4X
/* these macro describe send or recieve allowed */
#define SEND 								1 		// send allowed
#define RECIEVE 							0 		// recieve allowed
#define SlaveID 							0x01 	// the slave's ID
#define SendRecieveDelay 					20 		// Send turn to Recieve or Recieve turn to Send delay time vlaue
#define MaxDataLen 							64 	    // preset CommBuf array Max lenght
#define MaxRegLen 							128 	// preset SlaveOutputBuf Max lenght

//======================������=====================================
#define HMI_TASK_PRIO					4										//�������ȼ�
#define HMI_TASK_STK_SIZE 		512  								//�����ջ��С
#define HMI_TEST_TASK_PRIO		4										//�������ȼ�
#define HMI_TEST_STK_SIZE 		512  								//�����ջ��С



extern TaskHandle_t HMI_Run_Task_Handler;					//��ȫ������
extern TaskHandle_t HMI_Update_task_Handler;					//��ȫ������
//==============�����Ȧ״̬�����򣬾�����modbuS.c�Լ�modbuS.h����ʹ��======================
extern unsigned char CoilDat[10];      						//�󶨶���д��Ȧ��һ�𣬶���д��ȦʱHMI�Ͽؼ���ַ����һ��	
//================MODBUS===========================
extern unsigned char  RecFinishF;
extern unsigned int   CommIndex;   								//�����������Ŵ���
extern unsigned char  writeadr; 									//���Ĵ������������õ���
extern unsigned int   CommIndexEnd;	   						//�ظ�HMI�������Ӧ���鷢��������
extern unsigned char  CommBuf[MaxDataLen];	  		//������������

extern unsigned char 	APPRecFinishF;

extern unsigned char  SlaveOutputBuf[MaxRegLen]; 	// Hight 8 bit of word is front,Low 8 bit is back

extern unsigned char  HMIErrorTime;
extern unsigned char  MasterErrorTime;
extern unsigned char 	MasterPointer;
extern unsigned char  APPSendFinish;  //MODBUS �������




unsigned int crc16_hmi(unsigned char *puchMsg,unsigned int usDataLen);	//CRCУ��
void Modbus_Configuration(void);		 						//�Ĵ�����ʼ��
void AnalyzeRecieve(void);			 			//������������
void HMI_Run_task(void *pvParameters);
#endif
