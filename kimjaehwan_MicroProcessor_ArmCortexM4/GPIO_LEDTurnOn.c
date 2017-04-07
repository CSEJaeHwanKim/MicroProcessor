#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"


void Init_GPIO() {

	GPIO_InitTypeDef GPIO_InitStructure;

	//다음과 같이 선언이 가능함.GPIO는 최대 100MHz까지 출력이 가능함. 

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);// GPIOA포트를 Enable하게 함.

	//RCC는 Reset and Clock Control의 약자이며 ClockSetting이나 Reset을 할때 사용됨

	//AHB은 AHB1, AHB2로 나뉘고, APB또한 APB1, APB2로 나뉨

	//AHB는 Advanced High-performance Bus, APB는 Advanced Peripheral Bus (APB)의 약어임.

	// STM32F4 DataSheet 2.2 DeviceoverView에 따르면, GPIO PORT A~ I까지 AHB에 연결되어있음.

	//AHB1은 기본 100 MHZ로 고정되어있고, AHB2는 168MHZ/2의 속도까지 세팅이 가능함.

	//http://www.sciencezero.org/files/e/ee/STM32F4DeviceOverview.jpg 참조


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIO 15번 핀으로 초기화 이핀은 보드 내의 붉은색 내부 LED와 연결되어있음.

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Pin의 모드를 OUT으로 설정 (읽기로 사용하고 싶으면 GPIO_Mode_IN을 활용)

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO의 PULL-UP, PULL-DOWN을 설정 안함.

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //GPIO의 OType를 PP로 설정

	//PP는 Push-Pull OD는 Open-Drain을 의미 

	//링크 참조 : http://forum.falinux.com/zbxe/index.php?document_srl=826690&mid=hardware 

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //클럭 셋팅 2MHz, 25MHz, 50MHz, 100MHz 네개 설정이 가능함.

	GPIO_Init(GPIOE, &GPIO_InitStructure); //GPIO A포트에 설정된 GPIO 구조체를 GPIO_Init함수에 전달하여 초기화 완료
}

int main()

{

	Init_GPIO(); //GPIO를 초기화시킴
	int i = 0;//변수 선언
	while (1)
	{
		if (i % 2 == 0)

			GPIO_SetBits(GPIOE, GPIO_Pin_2); //Low 레지스터 비트를 0으로 바꿈

		else

			GPIO_ResetBits(GPIOE, GPIO_Pin_2); //High 레지스터 비트를 초기화시킴 <- 디폴트 상태

		i = 1 - i;

		delay_ms(50); // Delay를 실행.1초

	}

}




