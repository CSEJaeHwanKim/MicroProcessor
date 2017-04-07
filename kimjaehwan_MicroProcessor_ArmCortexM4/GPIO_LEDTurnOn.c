#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"


void Init_GPIO() {

	GPIO_InitTypeDef GPIO_InitStructure;

	//������ ���� ������ ������.GPIO�� �ִ� 100MHz���� ����� ������. 

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);// GPIOA��Ʈ�� Enable�ϰ� ��.

	//RCC�� Reset and Clock Control�� �����̸� ClockSetting�̳� Reset�� �Ҷ� ����

	//AHB�� AHB1, AHB2�� ������, APB���� APB1, APB2�� ����

	//AHB�� Advanced High-performance Bus, APB�� Advanced Peripheral Bus (APB)�� �����.

	// STM32F4 DataSheet 2.2 DeviceoverView�� ������, GPIO PORT A~ I���� AHB�� ����Ǿ�����.

	//AHB1�� �⺻ 100 MHZ�� �����Ǿ��ְ�, AHB2�� 168MHZ/2�� �ӵ����� ������ ������.

	//http://www.sciencezero.org/files/e/ee/STM32F4DeviceOverview.jpg ����


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIO 15�� ������ �ʱ�ȭ ������ ���� ���� ������ ���� LED�� ����Ǿ�����.

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Pin�� ��带 OUT���� ���� (�б�� ����ϰ� ������ GPIO_Mode_IN�� Ȱ��)

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO�� PULL-UP, PULL-DOWN�� ���� ����.

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //GPIO�� OType�� PP�� ����

	//PP�� Push-Pull OD�� Open-Drain�� �ǹ� 

	//��ũ ���� : http://forum.falinux.com/zbxe/index.php?document_srl=826690&mid=hardware 

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //Ŭ�� ���� 2MHz, 25MHz, 50MHz, 100MHz �װ� ������ ������.

	GPIO_Init(GPIOE, &GPIO_InitStructure); //GPIO A��Ʈ�� ������ GPIO ����ü�� GPIO_Init�Լ��� �����Ͽ� �ʱ�ȭ �Ϸ�
}

int main()

{

	Init_GPIO(); //GPIO�� �ʱ�ȭ��Ŵ
	int i = 0;//���� ����
	while (1)
	{
		if (i % 2 == 0)

			GPIO_SetBits(GPIOE, GPIO_Pin_2); //Low �������� ��Ʈ�� 0���� �ٲ�

		else

			GPIO_ResetBits(GPIOE, GPIO_Pin_2); //High �������� ��Ʈ�� �ʱ�ȭ��Ŵ <- ����Ʈ ����

		i = 1 - i;

		delay_ms(50); // Delay�� ����.1��

	}

}




