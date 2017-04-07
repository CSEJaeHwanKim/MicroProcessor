
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#define TIMER_PRESCALER_FREQ 1000000
#define TIMER_FREQ 100000

int count = 0; // ī��Ʈ
int distance = 0; // �Ÿ�
int i = 0; // Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 ��Ϲ��� ����
int state = 0; // HIGH or Low ���� �Ǻ�

/* Trigger�� ���� GPIO�� �ʱ�ȭ */
void Init_Trigger()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // Ʈ���Ÿ� ����� �ɹ�ȣ = GPIOE 0����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // ��� OUT [�ɹ�ȣ 0]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIO�� �ʱ�ȭ
}

/* Echo�� ���� GPIO�� �ʱ�ȭ */
void Init_Echo()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // ���ڸ� ����� �ɹ�ȣ = GPIOE 1����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // ��� IN [�ɹ�ȣ 1]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //GPIO�� �ʱ�ȭ
}

void Init_DAC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // DAC�� ����Ͽ� ������ ų �ɹ�ȣ = GPIOA 4����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIO�� �ʱ�ȭ
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None; // Ʈ���� ��带 ����Ұ��ΰ� 
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable; // ���� ���
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;// �ִ� Amplitude �� ����
	DAC_Init(DAC_Channel_1, &DAC_InitStructure); // DAC ����ü �ʱ�ȭ
	DAC_SetChannel1Data(DAC_Align_12b_R, 0); // �ʱⰪ = 0 
	DAC_Cmd(DAC_Channel_1, ENABLE); // ����
}

/* Ÿ�̸� ���� �κ� */
void Init_Timer()
{

	uint16_t PrescalerValue; // Prescaler
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // Ÿ�̸� 2�� ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	SystemCoreClockUpdate();
	PrescalerValue = (uint16_t)(SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

void Init_Timer3()
{

	uint16_t PrescalerValue; // Prescaler
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // Ÿ�̸� 2�� ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	SystemCoreClockUpdate();
	PrescalerValue = (uint16_t)(SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}



/* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (i == 0)//trigger
		{
			GPIOE->BSRRL = GPIO_Pin_0;
			state = 0; // ���� 0
			i = 1;
		}
		if (i == 1)
		{
			//GPIO_ResetBits(GPIOE,GPIO_Pin_0); // Ʈ���Ÿ� high���·� ���۽�Ŵ.
			GPIOE->BSRRH = GPIO_Pin_0;
			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1))//echo�� ���� �о���϶� ���� ������
			{
				//   GPIO_SetBits(GPIOA,GPIO_Pin_4); // ���� Ű�� ���Ͽ� DAC�� �Ҵ�� �ɹ�ȣ 4�� Set
				count++; // ���ڰ� ������ count ����
				state = 1; //���� 1

			}
			else if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0 && state == 1) // ����ܰ迡�� echo�� �� �о echo�� ���´� 0�ε� �̹� FALLING EDGE�� ������ �� (�ٽ� TRIGGER�� �����ϱ� �� ���� ����� �� ��),
			{
				distance = (count * 20); // �Ÿ����

				if (distance >= 4095){
					distance = 4095;
				}
				DAC_SetChannel1Data(DAC_Align_12b_R, distance);
				//  GPIO_ResetBits(GPIOA,GPIO_Pin_4); // ���� ����
				//GPIO_SetBits(GPIOA,GPIO_Pin_4); // ���� Ű�� ���Ͽ� DAC�� �Ҵ�� �ɹ�ȣ 4�� Set

				count = 0; // count 0 �ʱ�ȭ
				state = 0; // ���� 0 �ʱ�ȭ

			}
		}
	}
}

/* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 3 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if (state == 0 && count == 0){
			i = 0;
		}
	}
}


int main()
{
	Init_DAC();
	Init_Trigger();
	Init_Echo();

	Init_Timer();
	Init_Timer3();
	while (1){}
}
