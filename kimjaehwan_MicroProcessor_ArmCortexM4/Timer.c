#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#define TIMER_PRESCALER_FREQ  1000000
#define TIMER_FREQ 100 

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
void Init_ADC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC channel 10 (PC0) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC channel configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b; //10bit change
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
	ADC_Cmd(ADC1, ENABLE);
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
int adc_result = 0;

void TIM2_IRQHandler(void)//타이머 인터럽트 핸들러
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		ADC_SoftwareStartConv(ADC1); // ADC 시작
		if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET) // ADC 완료까지 대기
		{
			adc_result = ADC_GetConversionValue(ADC1);//ADC값 추출
			i = adc_result;//i의 값에 adc_result값을 대입
			if (i >= 0 && i <= 100)//0==>아날로그신호 0~100
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);

			}
			else if (i >= 101 && i <= 200)//1==>아날로그신호 101~200
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_1);

			}
			else if (i >= 201 && i <= 300)//2==>아날로그신호 201~300
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);

			}
			else if (i >= 301 && i <= 400)//3==>아날로그신호 301~400
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);

			}
			else if (i >= 401 && i <= 500)//4==>아날로그신호 401~500
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_3);

			}
			else if (i >= 501 && i <= 600)//5==>아날로그신호 501~600
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_3);

			}
			else if (i >= 601 && i <= 700)//6==>아날로그신호 601~700
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_3);

			}
			else if (i >= 701 && i <= 800)//7==>아날로그신호 701~800
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_3);

			}
			else if (i >= 801 && i <= 900)//8==>아날로그신호 801~900
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_4);

			}
			else if (i >= 901 && i <= 1023)//9==>아날로그신호 901~1023
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
			}
		}

	}
}
int main()
{
	Init_GPIO();
	Init_ADC();
	Init_Timer();
	while (1){
	}
}
