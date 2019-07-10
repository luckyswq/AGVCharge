/**
  ******************************************************************************
  * @�ļ���     �� canopen_drv.c
  * @����       �� FelixWu
  * @�汾       �� V1.0.0
  * @����       �� 2019��03��01��
  * @ժҪ       �� nvidia com with arm����Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2019-03-01 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "pccom_drv.h"





/* ��ʱ��TIM��ر��� */

/* ��̬���� ------------------------------------------------------------------*/

/* Nvidia serial msg */
uint8_t  PC_COM_RX_BUF[PC_COM_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t PC_COM_RX_STA=0;       			//����״̬���

uint8_t  PC_COM_RX_BUF_BACKUP[PC_COM_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�


/************************************************
�������� �� UART_PCcom_Configuration
��    �� �� PCcom ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void UART_PCcom_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* ʱ������ */
    RCC_APB2PeriphClockCmd(PC_COM_TX_GPIO_CLK | PC_COM_RX_GPIO_CLK , ENABLE);

    if(USART1 == PC_COM)
        RCC_APB2PeriphClockCmd(PC_COM_CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(PC_COM_CLK, ENABLE);

    /* �������� */
    GPIO_InitStructure.GPIO_Pin = PC_COM_TX_PIN;                    //UART Tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PC_COM_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PC_COM_RX_PIN;                    //UART Rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(PC_COM_RX_GPIO_PORT, &GPIO_InitStructure);

    /* NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannel = PC_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PC_COM_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* UART���� */
    USART_InitStructure.USART_BaudRate = PC_COM_BaudRate;           //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //����λ��
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             //ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No ;               //У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ����� USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(PC_COM, &USART_InitStructure);

    USART_ClearFlag(PC_COM, USART_FLAG_RXNE | USART_FLAG_TC);
    USART_ITConfig(PC_COM, USART_IT_RXNE, ENABLE);                  //�����ж�
    USART_ITConfig(PC_COM, USART_IT_IDLE, ENABLE);                  //�����ж�

    //USART_Cmd(PC_COM, ENABLE);                                      //ʹ��UART
    USART_Cmd(PC_COM, DISABLE);                                      //ʹ��UART
}

/************************************************
�������� �� PC_SendByte
��    �� �� ���Դ��ڷ���һ���ֽ�
��    �� �� Data --- ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void PC_SendByte(uint8_t Data)
{
    /* �ȴ�����Ϊ�� */
    while(RESET == USART_GetFlagStatus(PC_COM, USART_FLAG_TXE));
    USART_SendData(PC_COM, Data);
}

/************************************************
�������� �� PC_SendNByte
��    �� �� ���Դ��ڷ���N���ֽ�
��    �� �� pData ---- ����
            Length --- ����
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void PC_SendNByte(uint8_t *pData, uint16_t Length)
{
//  taskENTER_CRITICAL();
    while(Length--)
    {
        PC_SendByte(*pData);
        pData++;
    }
//  taskEXIT_CRITICAL();
}

/************************************************
�������� �� PCcom_Driver_Init
��    �� �� PCcom������ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� FelixWu
*************************************************/
void PCcom_Driver_Init(void)
{

    UART_PCcom_Configuration();
}



/**** Copyright (C)2019 FelixWu. All Rights Reserved **** END OF FILE ****/