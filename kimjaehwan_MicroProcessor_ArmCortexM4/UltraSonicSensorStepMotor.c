
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#define TIMER_PRESCALER_FREQ 1000000
#define TIMER_FREQ 100000
#define TIMER_FREQ_car 1000

//int count = 0 ; // 카운트
//int distance  = 0; // 거리
int i = 0; // 타이머 인터럽트 핸들러 톱니바퀴 변수
int a_state = 0; // HIGH or Low 상태 판별
int b_state = 0;
int c_state = 0;
int a_count = 0;
int b_count = 0;
int c_count = 0;
int a_distance = 0;
int b_distance = 0;
int c_distance = 0;
int max = 0;
int f = 0;
int r = 0;
int l = 0;
int a = 0;
int b = 0;
int c = 0;
char buffer[32];
char s;

/* 스텝모터를 구동하기 위한 GPIO 선언 부분 */
void Init_GPIO()
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
} // ok


/* Trigger를 위한 GPIO의 초기화 */
void Init_TriggerA()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  // 트리거를 사용할 핀번호 = GPIOE 0번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 모드 OUT [핀번호 0]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIO핀 초기화
}

/* Echo를 위한 GPIO의 초기화 */
void Init_EchoA()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // 에코를 사용할 핀번호 = GPIOE 1번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 모드 IN [핀번호 1]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //GPIO핀 초기화
}

/* Trigger를 위한 GPIO의 초기화 */
void Init_TriggerB()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // 트리거를 사용할 핀번호 = GPIOE 0번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 모드 OUT [핀번호 0]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIO핀 초기화
}

/* Echo를 위한 GPIO의 초기화 */
void Init_EchoB()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  // 에코를 사용할 핀번호 = GPIOE 1번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 모드 IN [핀번호 1]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //GPIO핀 초기화
}

/* Trigger를 위한 GPIO의 초기화 */
void Init_TriggerC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // 트리거를 사용할 핀번호 = GPIOE 0번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 모드 OUT [핀번호 0]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIO핀 초기화
}

/* Echo를 위한 GPIO의 초기화 */
void Init_EchoC()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  // 에코를 사용할 핀번호 = GPIOE 1번핀
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 모드 IN [핀번호 1]
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //GPIO핀 초기화
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

void Init_Timer5(void){

	uint16_t PrescalerValue; // Prescaler
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; // 타이머 2에 선언
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	SystemCoreClockUpdate();
	PrescalerValue = (uint16_t)(SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ_car - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM5, ENABLE);
}

/* 타이머 인터럽트 핸들러 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (a == 0)//trigger
		{
			GPIOE->BSRRL = GPIO_Pin_8;
			a_state = 0; // 상태 0
			a = 1;
		}
		if (a == 1)
		{
			GPIOE->BSRRH = GPIO_Pin_8;

			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9))//echo의 값을 읽어들일때 값이 들어오면
			{
				a_count++; // 에코가 켜지면 count 증가
				a_state = 1; //상태 1
			}
			else if ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0 && a_state == 1))
			{
				a_distance = (a_count * 10); // 거리계산
				if (a_distance >= 4095){
					a_distance = 4095;


				}
				a_count = 0; // count 0 초기화
				a_state = 0; // 상태 0 초기화
			}
		}
		if (b == 0)//trigger
		{
			GPIOE->BSRRL = GPIO_Pin_10;
			b_state = 0; // 상태 0
			b = 1;
		}
		if (b == 1)
		{
			GPIOE->BSRRH = GPIO_Pin_10;

			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11))//echo의 값을 읽어들일때 값이 들어오면
			{
				b_count++; // 에코가 켜지면 count 증가
				b_state = 1; //상태 1
			}
			else if ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0 && b_state == 1))
			{
				b_distance = (b_count * 10); // 거리계산
				if (b_distance >= 4095){
					b_distance = 4095;

				}

				b_count = 0; // count 0 초기화
				b_state = 0; // 상태 0 초기화
			}
		}
		if (c == 0)//trigger
		{
			GPIOE->BSRRL = GPIO_Pin_12;
			c_state = 0; // 상태 0
			c = 1;
		}
		if (c == 1)
		{
			GPIOE->BSRRH = GPIO_Pin_12;

			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13))//echo의 값을 읽어들일때 값이 들어오면
			{
				c_count++; // 에코가 켜지면 count 증가
				c_state = 1; //상태 1
			}
			else if ((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0 && c_state == 1))
			{
				c_distance = (c_count * 10); // 거리계산

				if (c_distance >= 4095){
					c_distance = 4095;

				}

				c_count = 0; // count 0 초기화
				c_state = 0; // 상태 0 초기화 
			}
		}
	}


	if (a_distance > b_distance)
	{
		max = a_distance;
		s = 'F';
	}
	else
	{
		max = b_distance;
		s = 'R';
	}

	if (max > c_distance)
	{
		max = max;
	}

	else {
		max = c_distance;
		s = 'L';
	}

}

void Init_USART(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Configure UART peripheral
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	// Enable USART receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

/* 유아트 puts 함수 */
void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
	while (*s)
	{
		while (!(USARTx->SR & 0x00000040));
		USART_SendData(USARTx, *s);
		*s++;
	}
}

/* 유아트 핸들러 함수 */
void USART1_IRQHandler(void)//USART Handler
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // RX에 무슨 값이 들어왔는가?를 확인하는 방법.
	{
		char c = USART_ReceiveData(USART1);//마이크로 컨트롤러 내부의 RX에 일정 값이 들어모녀 한글자, 한글자씩 저장함.
	}
}

/* 타이머 인터럽트 핸들러 3 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if (a_state == 0 && a_count == 0)
		{
			a = 0;
		}
		if (b_state == 0 && b_count == 0)
		{
			b = 0;
		}
		if (c_state == 0 && c_count == 0)
		{
			c = 0;
		}
	}
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		sprintf(buffer, "%d %d %d %d %c\n", a_distance, b_distance, c_distance, max, s);
		USART_puts(USART1, buffer);

		if (max == a_distance)
		{
			if (f == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_0);
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
				f++;
			}
			else if (f == 1)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
				f++;
			}
			else if (f == 2)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				f++;
			}
			else if (f == 3)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_6);
				f++;
			}
			else if (f == 4)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_SetBits(GPIOE, GPIO_Pin_0);
				GPIO_SetBits(GPIOE, GPIO_Pin_7);
				f = 0;
				max = 0;
			}

		}

		if (max == b_distance)
		{
			if (r == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_0);
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
				r++;
			}
			else if (r == 1)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_3);
				r++;
			}
			else if (r == 2)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
				r++;
			}
			else if (r == 3)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				r++;
			}
			else if (r == 4)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_0);
				r = 0;
				max = 0;
			}

		}
		if (max == c_distance)
		{
			if (l == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_0);
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
				l++;
			}
			else if (l == 1)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
				l++;
			}
			else if (l == 2)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				l++;
			}
			else if (l == 3)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_SetBits(GPIOE, GPIO_Pin_6);
				l++;
			}
			else if (l == 4)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_SetBits(GPIOE, GPIO_Pin_7);
				l = 0;
				max = 0;
			}

		}
	}
}


int main()
{
	Init_USART();
	Init_TriggerA();
	Init_EchoA();
	Init_TriggerB();
	Init_EchoB();
	Init_TriggerC();
	Init_EchoC();
	Init_GPIO();
	Init_Timer();
	Init_Timer3();
	Init_Timer5();
	while (1){}
}
