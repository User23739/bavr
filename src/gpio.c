/*
* gpio.c
 *
 *  Created on: 29 авг. 2018 г.
 *      Author: a.boyarkin
 */

#include "main.h"


//--глобальная переменная
short flag_priori_chann_manual = 0;			// 0 - канал А; 1 - канал В.


void InitGPIO(void){

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUT1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUT1_PORT, &GPIO_InitStructure);


}

void ButControl(void){
	uint8_t bit = 0;
	bit = GPIO_ReadInputDataBit(BUT1_PORT, BUT1);
	if(bit != 0){
		flag_priori_chann_manual = 0;
	}
	else{
	flag_priori_chann_manual = 1;
	}
}
