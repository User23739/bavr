/*
 * gpio.h
 *
 *  Created on: 29 авг. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef GPIO_H_
#define GPIO_H_

#define BUT1 					GPIO_Pin_9		///<Кнопка переключения приоритетов
#define BUT1_PORT				GPIOE			///<Порт управления кнопкой приоритета

#define LED1 					GPIO_Pin_14		///<Кнопка переключения приоритетов
#define LED1_PORT				GPIOE			///<Порт управления кнопкой приоритета


void InitGPIO(void);
void ButControl(void);

#endif /* GPIO_H_ */
