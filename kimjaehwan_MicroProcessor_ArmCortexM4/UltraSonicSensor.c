
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#define TIMER_PRESCALER_FREQ 1000000
#define TIMER_FREQ 100000

int count = 0; // 카운트
int distance = 0; // 거리
int i = 0; // 타이머 인터럽트 핸들러 톱니바퀴 변수
int state = 0; // HIGH or Low 상태 판별

/* Trigger를 위한 GPIO의 초기화 */
void Init_Trigger()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // 트리거를 사용할 핀번호 = GPIOE 0번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 모드 OUT [핀번호 0]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIO핀 초기화
}

/* Echo를 위한 GPIO의 초기화 */
void Init_Echo()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // 에코를 사용할 핀번호 = GPIOE 1번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 모드 IN [핀번호 1]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //GPIO핀 초기화
}

void Init_DAC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // DAC를 사용하여 전구를 킬 핀번호 = GPIOA 4번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIO핀 초기화
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None; // 트리거 모드를 사용할것인가 
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable; // 버퍼 사용
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;// 최대 Amplitude 를 설정
	DAC_Init(DAC_Channel_1, &DAC_InitStructure); // DAC 구조체 초기화
	DAC_SetChannel1Data(DAC_Align_12b_R, 0); // 초기값 = 0 
	DAC_Cmd(DAC_Channel_1, ENABLE); // 시작
}

/* 타이머 선언 부분 */
void Init_Timer()
{

	uint16_t PrescalerValue; // Prescaler
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 타이머 2에 선언
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
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // 타이머 2에 선언
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



/* 타이머 인터럽트 핸들러 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (i == 0)//trigger
		{
			GPIOE->BSRRL = GPIO_Pin_0;
			state = 0; // 상태 0
			i = 1;
		}
		if (i == 1)
		{
			//GPIO_ResetBits(GPIOE,GPIO_Pin_0); // 트리거를 high상태로 동작시킴.
			GPIOE->BSRRH = GPIO_Pin_0;
			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1))//echo의 값을 읽어들일때 값이 들어오면
			{
				//   GPIO_SetBits(GPIOA,GPIO_Pin_4); // 전구 키기 위하여 DAC에 할당된 핀번호 4번 Set
				count++; // 에코가 켜지면 count 증가
				state = 1; //상태 1

			}
			else if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0 && state == 1) // 실행단계에서 echo를 다 읽어서 echo의 상태는 0인데 이미 FALLING EDGE가 끝났을 때 (다시 TRIGGER가 동작하기 전 까지 대기모드 일 때),
			{
				distance = (count * 20); // 거리계산

				if (distance >= 4095){
					distance = 4095;
				}
				DAC_SetChannel1Data(DAC_Align_12b_R, distance);
				//  GPIO_ResetBits(GPIOA,GPIO_Pin_4); // 전구 끄고
				//GPIO_SetBits(GPIOA,GPIO_Pin_4); // 전구 키기 위하여 DAC에 할당된 핀번호 4번 Set

				count = 0; // count 0 초기화
				state = 0; // 상태 0 초기화

			}
		}
	}
}

/* 타이머 인터럽트 핸들러 3 */
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
