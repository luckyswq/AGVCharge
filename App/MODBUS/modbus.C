#include "modbus.h"
#include "bsp.h"
#include "led_app.h"
//=====================================================================================
/* variable for modbus is following */
unsigned char RecFinishF;
unsigned int  CommIndex;   								//�����������Ŵ���
unsigned char writeadr; 									//���Ĵ������������õ���
unsigned int  CommIndexEnd;	   						//�ظ�HMI�������Ӧ����?��������
unsigned char CommBuf[MaxDataLen];	  		//������������


unsigned char 	APPRecFinishF;

unsigned char SlaveOutputBuf[MaxRegLen]; 	// Hight 8 bit of word is front,Low 8 bit is back
//==============�����Ȧ״�?�����򣬾�����modbuS.c�Լ�modbuS.h����ʹ��======================
unsigned char CoilDat[10];      					//�󶨶���д��Ȧ��һ�𣬶���д��ȦʱHMI�Ͽؼ���ַ����һ��	
//==================================================
unsigned int Read1_Add;  //����Ȧ����ʼ��ַ
unsigned int Read1_Num;  //����Ȧ�ĳ���
unsigned int Read3_Add;  //���Ĵ�������ʼ��ַ
unsigned int Read3_Num;  //���Ĵ����ĳ���
unsigned int Read16_Add;  //д�Ĵ�������ʼ��ַ
unsigned int Read16_Num;  //д�Ĵ����ĳ���
unsigned int Legg;
unsigned int time;
unsigned int time1;
unsigned int time2;
unsigned int add;

union 
{             
	unsigned int IntBuf; 
	struct
	{            
		unsigned char charBuf[2];	
	}half;  
}charTOInt; 

//=====================================д���ݵ��м����?=================================
unsigned char HMIErrorTime;
unsigned char MasterErrorTime; 
unsigned char MasterPointer=0;
unsigned char APPSendFinish=0;  //MODBUS �������?
//============================������==================================================
TaskHandle_t HMI_Run_Task_Handler;					    //��ȫ������
TaskHandle_t HMI_Update_task_Handler;					//��ȫ������
//======================================================================================
unsigned int crc16_hmi(unsigned char *puchMsg,unsigned int usDataLen); // puchMsg message to calculate CRC upon
/* Table Of CRC Values for high-order byte */ 
static const  unsigned char auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40};
/* Table of CRC values for low-order byte */

	static unsigned char  auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40};


									 											

	unsigned int crc16_hmi(unsigned char *puchMsg,unsigned int usDataLen) // puchMsg message to calculate CRC upon
{ 
	// usDataLen quantity of bytes in message
	unsigned char uchCRCHi = 0xFF ; // high byte of CRC initialized
	unsigned char uchCRCLo = 0xFF ; // low byte of CRC initialized
	unsigned int  uIndex ; // will index into CRC lookup table
	while (usDataLen--) // Pass through message buffer
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; // calculate the CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (((unsigned int)(uchCRCLo) << 8) | uchCRCHi) ;
}



void Modbus_Configuration(void)		 //�Ĵ�����ʼ��
{
	unsigned char i;
	for (i=0;i<=250;i++)   //delay
	RecFinishF=0;
	CommIndex=0;
	CommIndexEnd=0xff; // First enlarge recieve lenght
	
	APPRecFinishF=0;
}


void AnalyzeRecieve(void)			 //������������
{
	unsigned int crc16tem;
	if (CommBuf[CommIndexSlaveID]==SlaveID) // The slave ID is matched
	{
		crc16tem=crc16_hmi(CommBuf,CommIndex-1); // calculate CRC check, erase two CRC byte
		if (crc16tem==(((unsigned int)(CommBuf[CommIndexEnd]) << 8) | CommBuf[CommIndexEnd-1]))
			{ 
			  LED1_TOGGLE();   //LED���?
				switch (CommBuf[CommIndexFunction])
					{
						case ReadCoilSta:   //0x01����Ȧ�߼�ֵ

						  Read1_Add=CommBuf[2]<<8 | CommBuf[3];
							Read1_Num=CommBuf[4]<<8 | CommBuf[5];

                            CommBuf[0]=0x01; // 0
							CommBuf[1]=0x01; // 1
							CommBuf[2]=Read1_Num / 0x08;	 //��Ӧ���ֽ�������Ҫ��Ҫ�ر�ע�⣬�㴨�����������������϶�����16����������ֻ�Ƕ�Ӧ����ʼ��ַ������ͬ�������ڴ�������Ȧʱ��Ҫȷ��2��������1�����ص��ֽ�����2�������ֽڵ���ʼ��ַ��

							if(CommBuf[2]==2)	//�ж���Ӧ���ص�������Ҫ����
							{

							     if(Read1_Add==0)	  //�ж���ʼ��ַ
						         {
						              CommBuf[3]=CoilDat[0];	//0~7
						              CommBuf[4]=CoilDat[1];	  //8~15
						         }

							     if(Read1_Add==16)
						         {
						              CommBuf[3]=CoilDat[2];	 //16~23
						              CommBuf[4]=CoilDat[3];	  //24~31
						         }

								 if(Read1_Add==32)
							     {
									  CommBuf[3]=CoilDat[4];	 //32~39
						              CommBuf[4]=CoilDat[5];	  //40~47
							     }

								 if(Read1_Add==48)
							     {
									  CommBuf[3]=CoilDat[6];	 //48~55
						              CommBuf[4]=CoilDat[7];	  //56~63
							     }

								 if(Read1_Add==64)
							     {
									  CommBuf[3]=CoilDat[8];	 //64~71
						              CommBuf[4]=CoilDat[9];	  //72~79
							     }
							crc16tem=crc16_hmi(CommBuf,5); //0��CRCЧ��֮ǰ��������0~4������5����������5 
							CommBuf[5]=(unsigned char)(crc16tem & 0x00ff);  
							CommBuf[6]=(unsigned char)(crc16tem>>8); 
							CommIndexEnd=7;	 //�����һ���������������ֱ��һ������ʾ���͸������������ݳ��ȣ�ͨ���Ƚ���ȷ���ģ���stm32f10x_it������˵��
							}


							if(CommBuf[2]==4)
							{

							 if(Read1_Add==0)
							 {
							   CommBuf[3]=CoilDat[0];
							   CommBuf[4]=CoilDat[1];
							   CommBuf[5]=CoilDat[2];
							   CommBuf[6]=CoilDat[3];
							   crc16tem=crc16_hmi(CommBuf,7);
							   CommBuf[7]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[8]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=9; 
							 }

							 if(Read1_Add==16)
							 {
							   CommBuf[3]=CoilDat[2];
							   CommBuf[4]=CoilDat[3];
							   CommBuf[5]=CoilDat[4];
							   CommBuf[6]=CoilDat[5];
							   crc16tem=crc16_hmi(CommBuf,7);
							   CommBuf[7]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[8]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=9; 
							 }

							 if(Read1_Add==32)
							 {
							   CommBuf[3]=CoilDat[4];
							   CommBuf[4]=CoilDat[5];
							   CommBuf[5]=CoilDat[6];
							   CommBuf[6]=CoilDat[7];
							   crc16tem=crc16_hmi(CommBuf,7);
							   CommBuf[7]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[8]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=9; 
							 }

							 if(Read1_Add==48)
							 {
							   CommBuf[3]=CoilDat[6];
							   CommBuf[4]=CoilDat[7];
							   CommBuf[5]=CoilDat[8];
							   CommBuf[6]=CoilDat[9];
							   crc16tem=crc16_hmi(CommBuf,7);
							   CommBuf[7]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[8]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=9; 
							 }
							}


						   
							if(CommBuf[2]==6)
							{
							 if(Read1_Add==0)
							 {
							   CommBuf[3]=CoilDat[0];
							   CommBuf[4]=CoilDat[1];
							   CommBuf[5]=CoilDat[2];
							   CommBuf[6]=CoilDat[3];
							   CommBuf[7]=CoilDat[4];
							   CommBuf[8]=CoilDat[5];
							   crc16tem=crc16_hmi(CommBuf,9);
							   CommBuf[9]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[10]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=11; 
							 }

							 if(Read1_Add==16)
							 {
							   CommBuf[3]=CoilDat[2];
							   CommBuf[4]=CoilDat[3];
							   CommBuf[5]=CoilDat[4];
							   CommBuf[6]=CoilDat[5];
							   CommBuf[7]=CoilDat[6];
							   CommBuf[8]=CoilDat[7];
							   crc16tem=crc16_hmi(CommBuf,9);
							   CommBuf[9]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[10]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=11; 
							 }

							 if(Read1_Add==32)
							 {
							   CommBuf[3]=CoilDat[4];
							   CommBuf[4]=CoilDat[5];
							   CommBuf[5]=CoilDat[6];
							   CommBuf[6]=CoilDat[7];
							   CommBuf[7]=CoilDat[8];
							   CommBuf[8]=CoilDat[9];
							   crc16tem=crc16_hmi(CommBuf,9);
							   CommBuf[9]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[10]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=11; 
							 }
							}


							
							if(CommBuf[2]==8)
							{
							 if(Read1_Add==0)
							 {
							   CommBuf[3]=CoilDat[0];
							   CommBuf[4]=CoilDat[1];
							   CommBuf[5]=CoilDat[2];
							   CommBuf[6]=CoilDat[3];
							   CommBuf[7]=CoilDat[4];
							   CommBuf[8]=CoilDat[5];
							   CommBuf[9]=CoilDat[6];
							   CommBuf[10]=CoilDat[7];
							   crc16tem=crc16_hmi(CommBuf,11);
							   CommBuf[11]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[12]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=13; 
							 }

							 if(Read1_Add==16)
							 {
							   CommBuf[3]=CoilDat[2];
							   CommBuf[4]=CoilDat[3];
							   CommBuf[5]=CoilDat[4];
							   CommBuf[6]=CoilDat[5];
							   CommBuf[7]=CoilDat[6];
							   CommBuf[8]=CoilDat[7];
							   CommBuf[9]=CoilDat[8];
							   CommBuf[10]=CoilDat[9];
							   crc16tem=crc16_hmi(CommBuf,11);
							   CommBuf[11]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[12]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=13; 
							 }

							}
						

							
							if(CommBuf[2]==10)
							{
							 if(Read1_Add==0)
							 {
							   CommBuf[3]=CoilDat[0];
							   CommBuf[4]=CoilDat[1];
							   CommBuf[5]=CoilDat[2];
							   CommBuf[6]=CoilDat[3];
							   CommBuf[7]=CoilDat[4];
							   CommBuf[8]=CoilDat[5];
							   CommBuf[9]=CoilDat[6];
							   CommBuf[10]=CoilDat[7];
							   CommBuf[11]=CoilDat[8];
							   CommBuf[12]=CoilDat[9];
							   crc16tem=crc16_hmi(CommBuf,13);
							   CommBuf[13]=(unsigned char)(crc16tem & 0x00ff);  
							   CommBuf[14]=(unsigned char)(crc16tem>>8);
							   CommIndexEnd=15; 
							 }
							}

							CommIndex=0;
							if(RecFinishF)SendHMIData(USART2,CommBuf,CommIndexEnd);
							else if(APPRecFinishF)AGVSendDataToAppUART(USART3,CommBuf,CommIndexEnd);
							break;
					
						case ForceSingleCoil:  //0x05д������Ȧ						   	
							if (CommBuf[4]==0xff)
							{
								switch(CommBuf[3])
								{
									case 0:  CoilDat[0]|=0x01;break;	
						            case 1:  CoilDat[0]|=0x02;break;	
						            case 2:	 CoilDat[0]|=0x04;break;
						            case 3:	 CoilDat[0]|=0x08;break;
						            case 4:  CoilDat[0]|=0x10; break;
						            case 5:  CoilDat[0]|=0x20; break;
						            case 6:  CoilDat[0]|=0x40; break;
						            case 7:  CoilDat[0]|=0x80; break;

								
									case 8:   CoilDat[1]|=0x01; break;	
						            case 9:   CoilDat[1]|=0x02; break;
						            case 10:  CoilDat[1]|=0x04; break;
						            case 11:  CoilDat[1]|=0x08; break;
						            case 12:  CoilDat[1]|=0x10; break;
						            case 13:  CoilDat[1]|=0x20; break;
						            case 14:  CoilDat[1]|=0x40; break;
						            case 15:  CoilDat[1]|=0x80; break;


									case 16:   CoilDat[2]|=0x01; break;	
						            case 17:   CoilDat[2]|=0x02; break;
						            case 18:  CoilDat[2]|=0x04; break;
						            case 19:  CoilDat[2]|=0x08; break;
						            case 20:  CoilDat[2]|=0x10; break;
						            case 21:  CoilDat[2]|=0x20; break;
						            case 22:  CoilDat[2]|=0x40; break;
						            case 23:  CoilDat[2]|=0x80; break;


									case 24:   CoilDat[3]|=0x01; break;	
						            case 25:   CoilDat[3]|=0x02; break;
						            case 26:  CoilDat[3]|=0x04; break;
						            case 27:  CoilDat[3]|=0x08; break;
						            case 28:  CoilDat[3]|=0x10; break;
						            case 29:  CoilDat[3]|=0x20; break;
						            case 30:  CoilDat[3]|=0x40; break;
						            case 31:  CoilDat[3]|=0x80; break;

									case 32:   CoilDat[4]|=0x01; break;	
						            case 33:   CoilDat[4]|=0x02; break;
						            case 34:  CoilDat[4]|=0x04; break;
						            case 35:  CoilDat[4]|=0x08; break;
						            case 36:  CoilDat[4]|=0x10; break;
						            case 37:  CoilDat[4]|=0x20; break;
						            case 38:  CoilDat[4]|=0x40; break;
						            case 39:  CoilDat[4]|=0x80; break;

									case 40:   CoilDat[5]|=0x01; break;	
						            case 41:   CoilDat[5]|=0x02; break;
						            case 42:  CoilDat[5]|=0x04; break;
						            case 43:  CoilDat[5]|=0x08; break;
						            case 44:  CoilDat[5]|=0x10; break;
						            case 45:  CoilDat[5]|=0x20; break;
						            case 46:  CoilDat[5]|=0x40; break;
						            case 47:  CoilDat[5]|=0x80; break;

									case 48:   CoilDat[6]|=0x01; break;	
						            case 49:   CoilDat[6]|=0x02; break;
						            case 50:  CoilDat[6]|=0x04; break;
						            case 51:  CoilDat[6]|=0x08; break;
						            case 52:  CoilDat[6]|=0x10; break;
						            case 53:  CoilDat[6]|=0x20; break;
						            case 54:  CoilDat[6]|=0x40; break;
						            case 55:  CoilDat[6]|=0x80; break;

									case 56:   CoilDat[7]|=0x01; break;	
						            case 57:   CoilDat[7]|=0x02; break;
						            case 58:  CoilDat[7]|=0x04; break;
						            case 59:  CoilDat[7]|=0x08; break;
						            case 60:  CoilDat[7]|=0x10; break;
						            case 61:  CoilDat[7]|=0x20; break;
						            case 62:  CoilDat[7]|=0x40; break;
						            case 63:  CoilDat[7]|=0x80; break;

									case 64:   CoilDat[8]|=0x01; break;	
						            case 65:   CoilDat[8]|=0x02; break;
						            case 66:  CoilDat[8]|=0x04; break;
						            case 67:  CoilDat[8]|=0x08; break;
						            case 68:  CoilDat[8]|=0x10; break;
						            case 69:  CoilDat[8]|=0x20; break;
						            case 70:  CoilDat[8]|=0x40; break;
						            case 71:  CoilDat[8]|=0x80; break;

									case 72:   CoilDat[9]|=0x01; break;	
						            case 73:   CoilDat[9]|=0x02; break;
						            case 74:  CoilDat[9]|=0x04; break;
						            case 75:  CoilDat[9]|=0x08; break;
						            case 76:  CoilDat[9]|=0x10; break;
						            case 77:  CoilDat[9]|=0x20; break;
						            case 78:  CoilDat[9]|=0x40; break;
						            case 79:  CoilDat[9]|=0x80; break;

									default:
									break;
								}
							}
							else
							{
								switch(CommBuf[3])
								{
								   case 0:  CoilDat[0]&=(~0x01);break;	
						           case 1: 	CoilDat[0]&=(~0x02);break;
						           case 2: 	CoilDat[0]&=(~0x04);break;
						           case 3: 	CoilDat[0]&=(~0x08);break;
						           case 4:  CoilDat[0]&=(~0x10); break;
						           case 5:  CoilDat[0]&=(~0x20); break;
						           case 6:  CoilDat[0]&=(~0x40); break;
						           case 7:  CoilDat[0]&=(~0x80); break;

								   case 8:   CoilDat[1]&=(~0x01); break;	 
						           case 9:   CoilDat[1]&=(~0x02); break;
						           case 10:  CoilDat[1]&=(~0x04); break;
						           case 11:  CoilDat[1]&=(~0x08); break;
						           case 12:  CoilDat[1]&=(~0x10); break;
						           case 13:  CoilDat[1]&=(~0x20); break;
						           case 14:  CoilDat[1]&=(~0x40); break;
						           case 15:  CoilDat[1]&=(~0x80); break;

								   case 16:  CoilDat[2]&=(~0x01); break;	
						           case 17:  CoilDat[2]&=(~0x02); break;
						           case 18:  CoilDat[2]&=(~0x04); break;
						           case 19:  CoilDat[2]&=(~0x08); break;
						           case 20:  CoilDat[2]&=(~0x10); break;
						           case 21:  CoilDat[2]&=(~0x20); break;
						           case 22:  CoilDat[2]&=(~0x40); break;
						           case 23:  CoilDat[2]&=(~0x80); break;

								   case 24:   CoilDat[3]&=(~0x01); break;	
						           case 25:   CoilDat[3]&=(~0x02); break;
						           case 26:  CoilDat[3]&=(~0x04); break;
						           case 27:  CoilDat[3]&=(~0x08); break;
						           case 28:  CoilDat[3]&=(~0x10); break;
						           case 29:  CoilDat[3]&=(~0x20); break;
						           case 30:  CoilDat[3]&=(~0x40); break;
						           case 31:  CoilDat[3]&=(~0x80); break;

								   case 32:   CoilDat[4]&=(~0x01); break;	
						           case 33:   CoilDat[4]&=(~0x02); break;
						           case 34:  CoilDat[4]&=(~0x04); break;
						           case 35:  CoilDat[4]&=(~0x08); break;
						           case 36:  CoilDat[4]&=(~0x10); break;
						           case 37:  CoilDat[4]&=(~0x20); break;
						           case 38:  CoilDat[4]&=(~0x40); break;
						           case 39:  CoilDat[4]&=(~0x80); break;

                                   case 40:   CoilDat[5]&=(~0x01); break;	
						           case 41:   CoilDat[5]&=(~0x02); break;
						           case 42:  CoilDat[5]&=(~0x04); break;
						           case 43:  CoilDat[5]&=(~0x08); break;
						           case 44:  CoilDat[5]&=(~0x10); break;
						           case 45:  CoilDat[5]&=(~0x20); break;
						           case 46:  CoilDat[5]&=(~0x40); break;
						           case 47:  CoilDat[5]&=(~0x80); break;

								   case 48:   CoilDat[6]&=(~0x01); break;	
						           case 49:   CoilDat[6]&=(~0x02); break;
						           case 50:  CoilDat[6]&=(~0x04); break;
						           case 51:  CoilDat[6]&=(~0x08); break;
						           case 52:  CoilDat[6]&=(~0x10); break;
						           case 53:  CoilDat[6]&=(~0x20); break;
						           case 54:  CoilDat[6]&=(~0x40); break;
						           case 55:  CoilDat[6]&=(~0x80); break;

								   case 56:   CoilDat[7]&=(~0x01); break;	
						           case 57:   CoilDat[7]&=(~0x02); break;
						           case 58:  CoilDat[7]&=(~0x04); break;
						           case 59:  CoilDat[7]&=(~0x08); break;
						           case 60:  CoilDat[7]&=(~0x10); break;
						           case 61:  CoilDat[7]&=(~0x20); break;
						           case 62:  CoilDat[7]&=(~0x40); break;
						           case 63:  CoilDat[7]&=(~0x80); break;

								   case 64:   CoilDat[8]&=(~0x01); break;	
						           case 65:   CoilDat[8]&=(~0x02); break;
						           case 66:  CoilDat[8]&=(~0x04); break;
						           case 67:  CoilDat[8]&=(~0x08); break;
						           case 68:  CoilDat[8]&=(~0x10); break;
						           case 69:  CoilDat[8]&=(~0x20); break;
						           case 70:  CoilDat[8]&=(~0x40); break;
						           case 71:  CoilDat[8]&=(~0x80); break;

								   case 72:   CoilDat[9]&=(~0x01); break;	
						           case 73:   CoilDat[9]&=(~0x02); break;
						           case 74:  CoilDat[9]&=(~0x04); break;
						           case 75:  CoilDat[9]&=(~0x08); break;
						           case 76:  CoilDat[9]&=(~0x10); break;
						           case 77:  CoilDat[9]&=(~0x20); break;
						           case 78:  CoilDat[9]&=(~0x40); break;
						           case 79:  CoilDat[9]&=(~0x80); break;

								   default:
								   break;
								}
							}
					    CommBuf[0]=0x01;
							CommBuf[1]=0x05;
							CommBuf[2]=CommBuf[CommIndexStartAdrHi];
							CommBuf[3]=CommBuf[CommIndexStartAdrLo];
							CommBuf[4]=CommBuf[CommIndexNoPointHi];
							CommBuf[5]=CommBuf[CommIndexNoPointLo];
							crc16tem=crc16_hmi(CommBuf,6); 					// then send low 8 bit data
							CommBuf[6]=(unsigned char)(crc16tem & 0x00ff); // send crccheck low 8 bit is front
							CommBuf[7]=(unsigned char)(crc16tem>>8); 		 // then send hight 8 bit
							CommIndexEnd=8;
							CommIndex=0;
							if(RecFinishF)SendHMIData(USART2,CommBuf,CommIndexEnd);
							else if(APPRecFinishF)AGVSendDataToAppUART(USART3,CommBuf,CommIndexEnd);
							break;
						case ReadHoldReg: //������Ĵ���?
							Read3_Add=CommBuf[2]<<8 | CommBuf[3];    //��ȡ����ʼ��ַ
							Read3_Num=CommBuf[4]<<8 | CommBuf[5];	 //��ȡ������
							Legg= Read3_Num * 2;
							CommBuf[CommIndexSlaveID]=SlaveID; // 0
							CommBuf[CommIndexFunction]=ReadHoldReg; // 1
							CommBuf[2]= Read3_Num * 2; // 2 Byte Count
							add=Read3_Add-200 ;   //��case�Ĳ�ֵ
							time1=add*2;		//case-200��ֵ��SlaveOutputBuf��һ��ֵ��2��
						  for(time=0;time<Legg;time++)CommBuf[time+3]=SlaveOutputBuf[time1++];
							time2=Legg+3;
              CommIndex=time2;																									 							
							crc16tem=crc16_hmi(CommBuf,CommIndex); // then send low 8 bit data
							CommBuf[CommIndex++]=(unsigned char)(crc16tem & 0x00ff); // send crccheck low 8 bit is front
							CommBuf[CommIndex]=(unsigned char)(crc16tem>>8); // then send hight 8 bit
							CommIndexEnd=CommIndex+1;
							CommIndex=0;
							if(RecFinishF)SendHMIData(USART2,CommBuf,CommIndexEnd);
							else if(APPRecFinishF)AGVSendDataToAppUART(USART3,CommBuf,CommIndexEnd);
							break;
						case PresetMulReg32:  //0x10д�Ĵ���
							Read16_Add=CommBuf[2]<<8 | CommBuf[3];
							Read16_Num=CommBuf[4]<<8 | CommBuf[5];
							Legg= Read16_Num * 2;
							CommBuf[CommIndexSlaveID]=SlaveID; // 0
							CommBuf[CommIndexFunction]=PresetMulReg32; // 1
							add=Read16_Add-200 ;   //��case�Ĳ�ֵ
							time1=add*2;		//case-200��ֵ��SlaveOutputBuf��һ��ֵ��2��
							for(time=0;time<Legg;time++)SlaveOutputBuf[time1++]=CommBuf[7+time]; // 4
							CommIndex=2;
							CommBuf[2]=CommBuf[CommIndexStartAdrHi]; // 2
							CommBuf[3]=CommBuf[CommIndexStartAdrLo]; // 3
							CommBuf[4]=CommBuf[CommIndexNoPointHi]; // 4
							CommBuf[5]=CommBuf[CommIndexNoPointLo]; // 5					
							crc16tem=crc16_hmi(CommBuf,6); // then send low 8 bit data
							CommBuf[6]=(unsigned char)(crc16tem & 0x00ff); // send crccheck low 8 bit is front
							CommBuf[7]=(unsigned char)(crc16tem>>8); // then send hight 8 bit
							CommIndexEnd=8;
							CommIndex=0;
							if(RecFinishF)SendHMIData(USART2,CommBuf,CommIndexEnd);
							else if(APPRecFinishF)AGVSendDataToAppUART(USART3,CommBuf,CommIndexEnd);
							break;
						default:
							vTaskDelay(10);
							CommIndex=0;
							break;
					}
			}
		else
		{
			//for (i=0;i<=SendRecieveDelay;i++);//Delayfor (i=0;i<=SendRecieveDelay;i++);//Delay	 8600
			vTaskDelay(10);
			CommIndex=0;
		}
	}
}



void HMI_Run_task(void *pvParameters){
	u32 NotifyValue;
	UART_HMI_Configuration(9600);
	UART_App_Configuration(9600);
	RecFinishF=1;RecFinishF=0;
	CommIndex=0;
	CommIndexEnd=0xff; // First enlarge recieve lenght 														//�ֶ�ģʽ	
	
	while(1)
	{
		vTaskDelay(100);
		NotifyValue=ulTaskNotifyTake(pdTRUE,portMAX_DELAY);	//��ȡ����֪ͨ	
		if(NotifyValue==1)
		{
			AnalyzeRecieve();//���ô���������Ӧд�Ĵ����Լ����Ĵ���	
			RecFinishF=0;
			APPRecFinishF=0;

			if((CoilDat[0]&0x01))
			{

				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))		//�缫���
				{
					CoilDat[0]|=(0x01<<4);
					GPIO_SetBits(GPIOB,GPIO_Pin_4);
				}
				else 
				{
					CoilDat[0]&=(~(0x01<<4));                       //�缫������λ
					GPIO_ResetBits(GPIOB,GPIO_Pin_4);
				}

				
			}
			else GPIO_ResetBits(GPIOB,GPIO_Pin_4);						 
//==============================================================
			if(CoilDat[0]&(0x01<<1))                       //�缫����
			{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))
				{
					CoilDat[0]|=(0x01<<5);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else 
				{
					CoilDat[0]&=(~(0x01<<5));
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
			}
			else GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		}
	}
}	


