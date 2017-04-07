#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#define TIMER_PRESCALER_FREQ  1000000
#define TIMER_FREQ 1 

void Init_GPIO()
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void Init_Timer()
{

	uint16_t PrescalerValue;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	SystemCoreClockUpdate();

	PrescalerValue = (uint16_t)(SystemCoreClock / TIMER_PRESCALER_FREQ) - 1;

	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;

	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM IT enable */

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM3 enable counter */

	TIM_Cmd(TIM2, ENABLE);

}

//Pin_1==A
//Pint_2==B
//Pin_3==C
//Pint_4==D

int i = 0;
void TIM2_IRQHandler(void)//Ÿ�̸� ���ͷ�Ʈ �ڵ鷯
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if (i == 0)//segement�� ���� 0�϶� 
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);//���� ���� RESET�Ѵ�
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_4);
			i++;
		}
		else if (i == 1)//segement�� ���� 1�϶�
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//Pin_1�� Ų��
			i++;
		}
		else if (i == 2)//segement�� ���� 2�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_2);//Pin_2�� Ų��
			i++;
		}
		else if (i == 3)//segement�� ���� 3�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//Pin_1�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_2);//Pin_2�� Ų��
			i++;
		}
		else if (i == 4)//segement�� ���� 4�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);//�������� RESET
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_3);//Pin_3�� Ų��
			i++;
		}
		else if (i == 5)//segement�� ���� 5�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//Pin_1�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_3);//Pin_3�� Ų��
			i++;
		}
		else if (i == 6)//segement�� ���� 6�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);//�������� RESET
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_2);//Pin_2�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_3);//Pin_3�� Ų��
			i++;
		}
		else if (i == 7)//segement�� ���� 7�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);//�������� RESET
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//Pin_1�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_2);//Pin_2�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_3);//Pin_3�� Ų��
			i++;
		}
		else if (i == 8)//segement�� ���� 8�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);//�������� RESET
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);//�������� RESET
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_4);//Pin_4�� Ų��
			i++;
		}
		else if (i == 9)//segement�� ���� 0�϶�
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_4);//�������� RESET
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//Pin_1�� Ų��
			GPIO_SetBits(GPIOE, GPIO_Pin_4);//Pin_4�� Ų��
			i = 0;//�������� ó�� ���·� ���ư��� ���� 0���� i ���� �ʱ�ȭ �����ش�
		}

	}

}
int main()
{
	Init_GPIO();
	Init_Timer();
	while (1){
	}
}
