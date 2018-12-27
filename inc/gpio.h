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

#define LED1 					GPIO_Pin_15		///
#define LED1_PORT				GPIOE			///
#define LED2 					GPIO_Pin_13		///
#define LED2_PORT				GPIOE			///


void InitGPIO(void);
void ButControl(void);

#endif /* GPIO_H_ */
