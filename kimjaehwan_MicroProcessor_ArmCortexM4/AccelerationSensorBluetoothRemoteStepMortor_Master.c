/*CREATED BY Byeon*/
/*2015.06.09*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"


#define TIMER_PRESCALER_FREQ        1000000          // timer 입력 클력 1MHz
#define TIMER_FREQ_2                 100//timer 2 주파수 1000Hz      //신호 제어    
#define TIMER_FREQ_3    100



void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

char cPC;
char cBT;
int pcState = 0;
int btState = 0;
char buffer_pc[256];
char buffer_bt[32];
//int t=0;

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
}
void Init_Timer()
{
	uint16_t PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM2 clock enable 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Compute the prescaler value 
	// timer base counter에 1MHz 입력.
	// TIMER2는 APB1에 연결되어 있다.
	// TIMER input clock은 APBx의 clock divider가 1인 경우 HCLK의 클럭이 그대로 쓰이고,
	//                                            1이 아닌 경우 HCLK의 클럭 /2가 사용된다.
	// 현재 소스 구성에서는 drv/system_stm32f4xx.c 파일의 SetSysClock()함수 내에서 APB1 clock은 HCLK /4로 설정하고 있다.
	//SystemCoreClockUpdate();
	PrescalerValue = (uint16_t)(SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ_2 - 1;           // 10000Hz timer
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
	uint16_t PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//SystemCoreClockUpdate();
	PrescalerValue = (uint16_t)(SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ_3 - 1;           // Hz timer
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
void Init_USART(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO 초기화용 Structure를 선언
	USART_InitTypeDef USART_InitStructure; // USART 초기화용  Structure를 선언
	NVIC_InitTypeDef NVIC_InitStructure; // NVIC 초기화용 Structure 선언

	// Enable peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //GPIOA를 Enable시킴 GPIOA는 AHB1에 위치
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1을 Enalble 시킴 USART1은 APB2에 위치

	// Configure USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // UART Interrupt을 등록함.핸들러는 TImer처럼 고정되어있음
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption 인터럽트 우선순위 설정
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub 인터럽트 우선순위 설정
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ채널의 cmd를 Enable함.
	NVIC_Init(&NVIC_InitStructure); // NVIC 초기화

	// GPIO AF config
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // USART(또는 UART)의 TX 핀번호
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // USART(또는 UART)의 RX 핀번호
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure UART peripheral
	USART_InitStructure.USART_BaudRate = 115200; //Baudrate설정.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 전송하는 패킷의 길이는 8bit씩 전송함을 명시
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // stopbit는 1개임을 의미
	USART_InitStructure.USART_Parity = USART_Parity_No; // 패리티 비트는 없다. 패리티 비트는 단지 에러가 있음을 알려줄뿐, 에러를 직접 고치지 못하기 때문에, 이는 UART데이터 전송속도에 안좋은 영향을 끼칠 것.(매우 긴 패킷에서만..)
	USART_InitStructure.USART_HardwareFlowControl // HardwareFlowControl을 없다고 설정
		= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode에서 RX, TX를 설정

	USART_Init(USART1, &USART_InitStructure); //  USART초기화

	// Enable USART receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}
void Init_USART2_BT(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO 초기화용 Structure를 선언
	USART_InitTypeDef USART_InitStructure; // USART 초기화용  Structure를 선언
	NVIC_InitTypeDef NVIC_InitStructure; // NVIC 초기화용 Structure 선언

	// Enable peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIOA를 Enable시킴 GPIOA는 AHB1에 위치
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART1을 Enalble 시킴 USART1은 APB2에 위치

	// Configure USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; // UART Interrupt을 등록함.핸들러는 TImer처럼 고정되어있음
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption 인터럽트 우선순위 설정
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub 인터럽트 우선순위 설정
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ채널의 cmd를 Enable함.
	NVIC_Init(&NVIC_InitStructure); // NVIC 초기화

	// GPIO AF config
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // USART(또는 UART)의 TX 핀번호
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // USART(또는 UART)의 RX 핀번호
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Configure UART peripheral
	USART_InitStructure.USART_BaudRate = 115200; //Baudrate설정.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 전송하는 패킷의 길이는 8bit씩 전송함을 명시
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // stopbit는 1개임을 의미
	USART_InitStructure.USART_Parity = USART_Parity_No; // 패리티 비트는 없다. 패리티 비트는 단지 에러가 있음을 알려줄뿐, 에러를 직접 고치지 못하기 때문에, 이는 UART데이터 전송속도에 안좋은 영향을 끼칠 것.(매우 긴 패킷에서만..)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // HardwareFlowControl을 없다고 설정

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode에서 RX, TX를 설정

	USART_Init(USART2, &USART_InitStructure); //  USART초기화

	// Enable USART receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}
int main()
{

	Init_GPIO();
	Init_USART();
	Init_USART2_BT();
	Init_Timer();
	Init_Timer3();

	while (1){}
}

int i = 1;
int adc_result1 = 0;
int adc_result2 = 0;
int adc_result3 = 0;
int x = 0;
int y = 0;
int z = 0;
char ch;
int f = 0;
int b = 0;
int l = 0;
int r = 0;
int sona = 0;
void TIM2_IRQHandler(void)//타이머 인터럽트 핸들러
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		/*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*//*전진*/
		if (ch == 'F')
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
			}
		}
		else if (ch == 'B')
		{
			if (b == 0)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_0);
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
				b++;
			}
			else if (b == 1)
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_0);
				GPIO_SetBits(GPIOE, GPIO_Pin_7);
				b++;
			}
			else if (b == 2)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_0);
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);
				GPIO_SetBits(GPIOE, GPIO_Pin_1);
				GPIO_SetBits(GPIOE, GPIO_Pin_6);
				b++;
			}
			else if (b == 3)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_1);
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				b++;
			}
			else if (b == 4)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				GPIO_SetBits(GPIOE, GPIO_Pin_3);
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
				b = 0;
			}
		}
		else if (ch == 'L')
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
			}
		}
		else if (ch == 'R')
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
			}

		}

	}
}

void TIM3_IRQHandler(void)//타이머 인터럽트 핸들러
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}

}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // RX에 무슨 값이 들어왔는가?를 확인하는 방법.
	{
		cPC = USART_ReceiveData(USART1);//마이크로 컨트롤러 내부의 RX에 일정 값이 들어모녀 한글자, 한글자씩 저장함.

		switch (cPC){
		case '0':
			pcState = 0;
			break;
		case '1':
			pcState = 1;
			break;
		default:
			pcState = 0;
			break;
		}
	}
}

void USART2_IRQHandler(void)
{
	while (USART_GetITStatus(USART2, USART_IT_RXNE)) // RX에 무슨 값이 들어왔는가?를 확인하는 방법.
	{
		cBT = USART_ReceiveData(USART2);//마이크로 컨트롤러 내부의 RX에 일정 값이 들어모녀 한글자, 한글자씩 저장함.
		ch = cBT;
		USART_SendData(USART1, ch); //오로지 한개의 문자만 보낼수 있음.     

	}

}
void USART_puts(USART_TypeDef* USARTx, volatile char *s){
	while (*s){
		// wait until data register is empty
		while (!(USARTx->SR & 0x00000040));
		USART_SendData(USARTx, *s);
		*s++;
	}
}
