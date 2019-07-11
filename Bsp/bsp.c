/**
  ******************************************************************************
  * @�ļ���     �� bsp.c
  * @����       �� Felix Wu
  * @�汾       �� V1.0.0
  * @����       �� 2018��11��14��
  * @ժҪ       �� �ײ�����Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2018-11-14 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "bsp.h"
#include "bsp_can.h"
#include "bsp_timer.h"
#include "bsp_usart.h"


/************************************************
�������� �� LED_Initializes
��    �� �� LED��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� Felix Wu
*************************************************/
void LED_Initializes(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ��ʱ�� */
  RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PA.4 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);						 //PA.4 �����
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);						 //PA.8 �����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);// 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PA.4 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
     
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//释放PA13\PA14\PA15。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PA.4 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_4);						 //PA.4 �����
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PA.5 �����
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);						 //PA.6 �����
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);						 //PA.7 �����
}

/************************************************
�������� �� BSP_Init
��    �� �� �ײ�������ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� Felix Wu
*************************************************/
void BSP_Init(void)
{
  USART_Initializes();                           //USART�ײ��ʼ��
}


/**** Copyright (C)2018 Felix Wu. All Rights Reserved **** END OF FILE ****/
