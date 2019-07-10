/* Includes ------------------------------------------------------------------*/
#include "stepper_drv.h"

/*
 * 本文件包含了：
 * 1. 步进电机所使用的RS485串口的端口初始化


 * This file contains:
 * 1. Init of the serial port used by Stepper

*/
void UART_Stepper_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* 时钟配置 */
    RCC_APB2PeriphClockCmd(STEPPER_COM_TX_GPIO_CLK | STEPPER_COM_RX_GPIO_CLK , ENABLE);

    if(USART1 == STEPPER_COM)
        RCC_APB2PeriphClockCmd(STEPPER_COM_CLK, ENABLE);
    else
        RCC_APB1PeriphClockCmd(STEPPER_COM_CLK, ENABLE);

    /* 引脚配置 */
    GPIO_InitStructure.GPIO_Pin = STEPPER_COM_TX_PIN;                    //UART Tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(STEPPER_COM_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STEPPER_COM_RX_PIN;                    //UART Rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(STEPPER_COM_RX_GPIO_PORT, &GPIO_InitStructure);

    /* NVIC配置 */
    NVIC_InitStructure.NVIC_IRQChannel = STEPPER_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = STEPPER_COM_Priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* UART配置 */
    USART_InitStructure.USART_BaudRate = STEPPER_COM_BaudRate;           //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;        //传输位数
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             //停止位
    USART_InitStructure.USART_Parity = USART_Parity_Odd ;               //校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;    //收发功能
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发功能
    USART_Init(STEPPER_COM, &USART_InitStructure);

    USART_ClearFlag(STEPPER_COM, USART_FLAG_RXNE | USART_FLAG_TC);
    USART_ITConfig(STEPPER_COM, USART_IT_RXNE, ENABLE);                  //接收中断
    USART_ITConfig(STEPPER_COM, USART_IT_IDLE, ENABLE);                  //接收中断

    //USART_Cmd(PC_COM, ENABLE);                                      //使能UART
    USART_Cmd(STEPPER_COM, DISABLE);                                      //使能UART

}



void stepper_test(void)
{
    /* 使用“直接数据运行”
    	号机号码： 01h ==> 1
    	功能代码： 10h
    	写入寄存器起始地址： 00 58h
    	写入寄存器数： 00 10h ==>16个
    	写入byte数： 20h ==> 32个

    	1. 运行数据No.： 00 00 00 00h ==> 0
    	2. 方式： 相对定位（00 00 00 02）
    	3. 位置： 00 00 21 24h ==> 8500 step
    	4. 速度： 00 00 07 D0h ==> 2000Hz
    	5. 起动/变速斜率： 00 00 05 DCh ==> 1.5kHz
    	6. 停止斜率： 00 00 05 DCh ==> 1.5kHz
    	7. 运行电流: 00 00 03 E8h ==> 100.0%

    	反映触发： 全部数据反映（00 00 00 01）
    	错误检查： 1C 08h

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

