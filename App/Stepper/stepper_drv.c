/* Includes ------------------------------------------------------------------*/
#include "stepper_drv.h"

/*
 * ���ļ������ˣ�
 * 1. ���������ʹ�õ�RS485���ڵĶ˿ڳ�ʼ��


 * This file contains:
 * 1. Init of the serial port used by Stepper

*/
void UART_Stepper_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* ʱ������ */
    RCC_APB2PeriphClockCmd(STEPPER_COM_TX_GPIO_CLK | STEPPER_COM_RX_GPIO_CLK , ENABLE);

    if(USART1 == STEPPER_COM)
        RCC_APB2PeriphClockCmd(STEPPER_COM_CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(STEPPER_COM_CLK, ENABLE);

    /* �������� */
    GPIO_InitStructure.GPIO_Pin = STEPPER_COM_TX_PIN;                    //UART Tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(STEPPER_COM_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STEPPER_COM_RX_PIN;                    //UART Rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(STEPPER_COM_RX_GPIO_PORT, &GPIO_InitStructure);

    /* NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannel = STEPPER_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = STEPPER_COM_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* UART���� */
    USART_InitStructure.USART_BaudRate = STEPPER_COM_BaudRate;           //������
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;        //����λ��
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             //ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_Odd ;               //У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;    //�շ�����
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ�����
    USART_Init(STEPPER_COM, &USART_InitStructure);

    USART_ClearFlag(STEPPER_COM, USART_FLAG_RXNE | USART_FLAG_TC);
    USART_ITConfig(STEPPER_COM, USART_IT_RXNE, ENABLE);                  //�����ж�
    USART_ITConfig(STEPPER_COM, USART_IT_IDLE, ENABLE);                  //�����ж�

    //USART_Cmd(PC_COM, ENABLE);                                      //ʹ��UART
    USART_Cmd(STEPPER_COM, DISABLE);                                      //ʹ��UART

}



void stepper_test(void)
{
    /* ʹ�á�ֱ���������С�
    	�Ż����룺 01h ==> 1
    	���ܴ��룺 10h
    	д��Ĵ�����ʼ��ַ�� 00 58h
    	д��Ĵ������� 00 10h ==>16��
    	д��byte���� 20h ==> 32��

    	1. ��������No.�� 00 00 00 00h ==> 0
    	2. ��ʽ�� ��Զ�λ��00 00 00 02��
    	3. λ�ã� 00 00 21 24h ==> 8500 step
    	4. �ٶȣ� 00 00 07 D0h ==> 2000Hz
    	5. ��/����б�ʣ� 00 00 05 DCh ==> 1.5kHz
    	6. ֹͣб�ʣ� 00 00 05 DCh ==> 1.5kHz
    	7. ���е���: 00 00 03 E8h ==> 100.0%

    	��ӳ������ ȫ�����ݷ�ӳ��00 00 00 01��
    	�����飺 1C 08h

    */
    uint8_t test_data[41] = {
        0x01,
        0x10,
        0x00, 0x58,
        0x00, 0x10,
        0x20,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x21, 0x34,
        0x00, 0x00, 0x07, 0xd0,
        0x00, 0x00, 0x05, 0xdc,
        0x00, 0x00, 0x05, 0xdc,
        0x00, 0x00, 0x03, 0xe8,
        0x00, 0x00, 0x00, 0x01,
        0x1c, 0x08
    };

    USART_SendNByte(USART3, test_data, 41);
    USART_SendNByte(USART2, test_data, 41);
}

