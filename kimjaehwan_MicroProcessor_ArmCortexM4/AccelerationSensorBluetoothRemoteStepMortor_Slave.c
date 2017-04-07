/*������� �����̺� �ڵ�*/
/*CREATED BY Byeon*/
/*2015.06.09*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"


#define TIMER_PRESCALER_FREQ        1000000          // timer �Է� Ŭ�� 1MHz
#define TIMER_FREQ_2         1000//timer 2 ���ļ� 1000Hz      //��ȣ ����    
#define TIMER_FREQ_3    1



void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

char cPC;
char cBT;
int pcState = 0;
int btState = 0;
char buffer_pc[32];
char buffer_bt[32];
/* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 ��Ϲ��� ���� */ /* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 ��Ϲ��� ���� */ /* Ÿ�̸� ���ͷ�Ʈ �ڵ鷯 ��Ϲ��� ���� */
int i = 1;
int adc_result1 = 0;
int adc_result2 = 0;
int adc_result3 = 0;
int x = 0;
int y = 0;
int z = 0;
char ch = 'a';
int f = 0;
int b = 0;
int l = 0;
int r = 0;
int sona = 0;



void Init_GPIO() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/* ADC1 ���� �κ�  [ X�� ] */
void Init_ADC1()
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
	ADC_Cmd(ADC1, ENABLE);
} // ok

/* ADC2 ���� �κ� [Y ��] */
void Init_ADC2()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC channel 10 (PC0) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC channel configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; //10bit change
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC2, &ADC_InitStructure);

	/* ADC1 regular channel10 configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC2, ENABLE);
} // ok

/* ADC3 ���� �κ� [Z ��] */
void Init_ADC3()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC channel 10 (PC0) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
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
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC1 regular channel10 configuration */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC3, ENABLE);
} // ok




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
	// timer base counter�� 1MHz �Է�.
	// TIMER2�� APB1�� ����Ǿ� �ִ�.
	// TIMER input clock�� APBx�� clock divider�� 1�� ��� HCLK�� Ŭ���� �״�� ���̰�,
	//                                            1�� �ƴ� ��� HCLK�� Ŭ�� /2�� ���ȴ�.
	// ���� �ҽ� ���������� drv/system_stm32f4xx.c ������ SetSysClock()�Լ� ������ APB1 clock�� HCLK /4�� �����ϰ� �ִ�.
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


void Init_USART(void){

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO �ʱ�ȭ�� Structure�� ����
	USART_InitTypeDef USART_InitStructure; // USART �ʱ�ȭ��  Structure�� ����
	NVIC_InitTypeDef NVIC_InitStructure; // NVIC �ʱ�ȭ�� Structure ����

	// Enable peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //GPIOA�� Enable��Ŵ GPIOA�� AHB1�� ��ġ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1�� Enalble ��Ŵ USART1�� APB2�� ��ġ

	// Configure USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // UART Interrupt�� �����.�ڵ鷯�� TImeró�� �����Ǿ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption ���ͷ�Ʈ �켱���� ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub ���ͷ�Ʈ �켱���� ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQä���� cmd�� Enable��.
	NVIC_Init(&NVIC_InitStructure); // NVIC �ʱ�ȭ

	// GPIO AF config
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // USART(�Ǵ� UART)�� TX �ɹ�ȣ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // USART(�Ǵ� UART)�� RX �ɹ�ȣ
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure UART peripheral
	USART_InitStructure.USART_BaudRate = 115200; //Baudrate����.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �����ϴ� ��Ŷ�� ���̴� 8bit�� �������� ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // stopbit�� 1������ �ǹ�
	USART_InitStructure.USART_Parity = USART_Parity_No; // �и�Ƽ ��Ʈ�� ����. �и�Ƽ ��Ʈ�� ���� ������ ������ �˷��ٻ�, ������ ���� ��ġ�� ���ϱ� ������, �̴� UART������ ���ۼӵ��� ������ ������ ��ĥ ��.(�ſ� �� ��Ŷ������..)
	USART_InitStructure.USART_HardwareFlowControl // HardwareFlowControl�� ���ٰ� ����
		= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode���� RX, TX�� ����

	USART_Init(USART1, &USART_InitStructure); //  USART�ʱ�ȭ

	// Enable USART receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void Init_USART2_BT(void){

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO �ʱ�ȭ�� Structure�� ����
	USART_InitTypeDef USART_InitStructure; // USART �ʱ�ȭ��  Structure�� ����
	NVIC_InitTypeDef NVIC_InitStructure; // NVIC �ʱ�ȭ�� Structure ����

	// Enable peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIOA�� Enable��Ŵ GPIOA�� AHB1�� ��ġ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART1�� Enalble ��Ŵ USART1�� APB2�� ��ġ

	// Configure USART Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; // UART Interrupt�� �����.�ڵ鷯�� TImeró�� �����Ǿ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption ���ͷ�Ʈ �켱���� ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub ���ͷ�Ʈ �켱���� ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQä���� cmd�� Enable��.
	NVIC_Init(&NVIC_InitStructure); // NVIC �ʱ�ȭ

	// GPIO AF config
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // USART(�Ǵ� UART)�� TX �ɹ�ȣ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // USART(�Ǵ� UART)�� RX �ɹ�ȣ
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Configure UART peripheral
	USART_InitStructure.USART_BaudRate = 115200; //Baudrate����.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �����ϴ� ��Ŷ�� ���̴� 8bit�� �������� ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // stopbit�� 1������ �ǹ�
	USART_InitStructure.USART_Parity = USART_Parity_No; // �и�Ƽ ��Ʈ�� ����. �и�Ƽ ��Ʈ�� ���� ������ ������ �˷��ٻ�, ������ ���� ��ġ�� ���ϱ� ������, �̴� UART������ ���ۼӵ��� ������ ������ ��ĥ ��.(�ſ� �� ��Ŷ������..)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // HardwareFlowControl�� ���ٰ� ����

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode���� RX, TX�� ����

	USART_Init(USART2, &USART_InitStructure); //  USART�ʱ�ȭ

	// Enable USART receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

int main()
{

	Init_GPIO();
	Init_ADC1();
	Init_ADC2();
	Init_ADC3();
	Init_USART();
	Init_USART2_BT();
	Init_Timer();
	Init_Timer3();
	while (1){
	}
}
void TIM2_IRQHandler(void)//Ÿ�̸� ���ͷ�Ʈ �ڵ鷯
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		ADC_SoftwareStartConv(ADC1);
		ADC_SoftwareStartConv(ADC2);
		ADC_SoftwareStartConv(ADC3);
		USART_SendData(USART2, ch);

		if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET) // ADC �Ϸ���� ���
		{
			adc_result1 = ADC_GetConversionValue(ADC1);// ADC1 �� ����
			x = adc_result1 / 5;
		}
		if (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == SET) // ADC �Ϸ���� ���
		{
			adc_result2 = ADC_GetConversionValue(ADC2);// ADC2 �� ����
			y = adc_result2 / 5;
		}
		if (ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET) // ADC �Ϸ���� ���
		{
			adc_result3 = ADC_GetConversionValue(ADC3);// ADC3 �� ����
			z = adc_result3 / 5;
		}

		//����//����//����//����//����//����//����//����//����//����//����//����//����//����//����//����
		if (x >= 480 && x <= 600)
		{
			ch = 'F';

		}


		//���� //���� //���� //���� //���� //���� //���� //���� //���� //���� //���� //���� //���� //����
		if (x >= 250 && x <= 350)
		{
			ch = 'B';

		}
		//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��//��ȸ��

		if ((y <= 350 && y >= 250))
		{
			ch = 'R';

		}
		//��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ�� //��ȸ��
		if ((y >= 420 && y <= 560))
		{
			ch = 'L';

		}
		//x,y,z
		//�� a , ���� b, ������ c, �� d

		//sprintf(buffer,"%d %d %d, %c\n",(int)x,(int)y,(int)z, ch); // String 
		// USART_puts(USART1, buffer); // ���ۿ� �������. �̷��� �ϸ� ComportMaster�� ����� �����մϴ�.
	}
}

void TIM3_IRQHandler(void)//Ÿ�̸� ���ͷ�Ʈ �ڵ鷯
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		USART_SendData(USART2, ch);
		if (i == 0)
		{
			GPIOA->BSRRH = GPIO_Pin_15;// �ռ� ��ϵ� �� ��������(GPIO_Pin_15)�� RESET(1)�� �ǰ���.                            
			i = 1;
		}
		else if (i == 1)
		{

			GPIOA->BSRRL = GPIO_Pin_15; // �ռ� ��ϵ� �� ��������(GPIO_Pin_15)�� SET(1)�� �ǰ���.            
			i = 0;
		}
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // RX�� ���� ���� ���Դ°�?�� Ȯ���ϴ� ���.
	{
		cPC = USART_ReceiveData(USART1);//����ũ�� ��Ʈ�ѷ� ������ RX�� ���� ���� ����� �ѱ���, �ѱ��ھ� ������.
		switch (cPC)
		{
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

int t = 0;
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) // RX�� ���� ���� ���Դ°�?�� Ȯ���ϴ� ���.
	{
		cBT = USART_ReceiveData(USART2);//����ũ�� ��Ʈ�ѷ� ������ RX�� ���� ���� ����� �ѱ���, �ѱ��ھ� ������.

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
