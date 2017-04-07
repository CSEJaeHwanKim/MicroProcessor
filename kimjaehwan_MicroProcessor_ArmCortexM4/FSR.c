
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#define TIMER_PRESCALER_FREQ 1000000
#define TIMER_FREQ 100000

int force = 0; // FSR�� �������� �з�


void Init_ADC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //ADC 1�� APB2�� ����Ѵ�. ���� ADC1�� GPIOC�� ������, �ΰ��� ���ÿ� Ŭ�� ������ ������Ѵ�. 
	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	/* Fsr�� ���� GPIO�� �ʱ�ȭ */
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMA�� ����� ���ΰ� ���Ұ��ΰ�? 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // ADC ���ø� ������ �����Ѵ�. 
	ADC_CommonInit(&ADC_CommonInitStructure); // ADC�� �ʱ�ȭ��Ŵ 
	/* ADC channel 10 (PC0) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // ADC1, 0�� PC0�̹Ƿ�, //   �̶��� �ʱ�ȭ��Ŵ. 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ADC channel configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 regular channel10 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //ADC ����
}

void Init_DAC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // DAC�� ����Ͽ� �������͸� ������ �ɹ�ȣ = GPIOA_Pin_4;
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


/* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		ADC_SoftwareStartConv(ADC1);
		if ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) == SET)
		{
			force = ADC_GetConversionValue(ADC1);//ADC�� ���� 
			if (force >= 4095)
			{
				force = 4095;
			}
			DAC_SetChannel1Data(DAC_Align_12b_R, force);

		}
	}
}


int main()
{
	Init_ADC();
	Init_DAC();
	Init_Timer();
	while (1){}
}
