/*
* gpio.c
 *
 *  Created on: 29 авг. 2018 г.
 *      Author: a.boyarkin
 */

#include "main.h"


//----глобальная переменная
short flag_priori_chann_manual = CHANNAL_A;			//перменная приоритета каналла


void InitGPIO(void){

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUT1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUT1_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = 		LED1;
	GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 		LED2;
	GPIO_InitStructure.GPIO_Mode = 		GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed =		GPIO_Speed_50MHz;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);

}

void ButControl(void){
	short bit = 0;
	bit = GPIO_ReadInputDataBit(BUT1_PORT, BUT1);
	if(!bit){
		flag_priori_chann_manual = CHANNAL_A;
	}
	else{
		flag_priori_chann_manual = CHANNAL_B;
	}
}
