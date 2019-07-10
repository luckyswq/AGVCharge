#include "HMI_dev.h"
#include "stm32f10x.h"
#include "modbus.h"
#include "led_app.h"

void SendHMIData(USART_TypeDef* USARTx,unsigned char *puchMsg,unsigned int usDataLen)
{
	unsigned char i;
	for (i = 0; i < usDataLen; ++i )
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){taskYIELD();}; 
		USART_SendData(USARTx, *puchMsg++);//�򴮿ڷ�������
	}
	//while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);  
}
			

void UART_HMI_Configuration(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);
	//UART4_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 																			//PC.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;											
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIOC.10		 
	//UART4_RX	  GPIOC.11��ʼ��											
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																				//PC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;														//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);																					//��ʼ��GPIOC.11
												
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;																//Usart4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=10;													//��ռ���ȼ�8  RTOS�ж�ע�����ȼ�����6--15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;															//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																									//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); 																				//��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);																	//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                     																	//ʹ�ܴ��� 
  USART_ClearFlag(USART2,USART_FLAG_TC);  
}

void USART2_IRQHandler(void)    //USART2_IRQHandler
{
BaseType_t xHigherPriorityTaskWoken;
		LED1_TOGGLE();   //LED???
		if (CommIndex<CommIndexEnd) // CommIndex less than recieve lenght
		{
			CommBuf[CommIndex]=USART_ReceiveData(USART2);	  //????????
//			Safety.HMITimerOut=0;
			if(CommIndex==5)
			{
				switch (CommBuf[CommIndexFunction])
				{
					case ReadCoilSta:
						CommIndexEnd=ReadCoilStaIndexEnd;
						break;
					case ReadBitSta:
						CommIndexEnd=ReadBitStaIndexEnd;
						break;
					case ForceSingleCoil:
						CommIndexEnd=ForceSingleCoilIndexEnd;
						break;
					case ReadHoldReg:
						CommIndexEnd=ReadHoldRegIndexEnd;
						break;
					case PresetMulReg32:
						CommIndexEnd=(CommBuf[4]<<8 | CommBuf[5])*2+8;
						break;
					default:
						break;
				}
			}
			CommIndex++;
		}
		else 
		{	
			CommBuf[CommIndexEnd]=USART_ReceiveData(USART2);
			RecFinishF=1;
			vTaskNotifyGiveFromISR(HMI_Run_Task_Handler,&xHigherPriorityTaskWoken);//??????????			
		}
    if( USART_GetITStatus(USART2, USART_IT_TC) == SET )
    {
        USART_ClearFlag(USART2, USART_FLAG_TC);
    }    
			
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)	
    {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
    USART_ClearFlag(USART2,USART_IT_RXNE); 
}
