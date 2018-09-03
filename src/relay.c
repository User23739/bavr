#include "relay.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"


//�������������




void RELAY_Init( void ){

	GPIO_InitTypeDef    GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	//1 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_1_PIN_IGBT | RELAY_1_PIN_ON | RELAY_1_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//2 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_2_PIN_IGBT | RELAY_2_PIN_ON | RELAY_2_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//3 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_3_PIN_IGBT | RELAY_3_PIN_ON | RELAY_3_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//4 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_4_PIN_IGBT | RELAY_4_PIN_ON | RELAY_4_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);


	//5 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_5_PIN_IGBT | RELAY_5_PIN_ON | RELAY_5_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	//6 ����
	GPIO_InitStruct.GPIO_Pin = 			(RELAY_6_PIN_IGBT | RELAY_6_PIN_ON | RELAY_6_PIN_OFF );
	GPIO_InitStruct.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOE, &GPIO_InitStruct);

}
/* ��������� ������ ������ ������
 * 1. ���������� / ������������ (������ ���������, � ����� ������ ������������� �� ������)
 * 2. ���������� / ����������� ��� ������� �� ������� (���� � ����)(������ ��������� �� ����� �� �������� ��� ������� ����)
 * 3. ������������ / ������������ (������ �� ���������, � ����� ������ ���������� �� ������)
 * 4. ������������ / ���������� ��� ����� �� ������� (������ �� ���������, �� ����� �� �������� ��� ������� ����)*/



