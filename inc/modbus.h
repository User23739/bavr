/*
 * modbus.h
 *
 *  Created on: 2 нояб. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef MODBUS_H_
#define MODBUS_H_


#include "main.h"

#define MaxLenRecBuf 250//pазмер буфера принимаемых данных
#define MaxLenTrBuf 250 //pазмер буфера передаваемых данных

#define TRUE 1
#define FALSE 0

#define Hi(int) (char) (int>>8)
#define Low(int) (char) (int)

#define Hi_short(short) (char) (short>>8)
#define Low_short(short) (char) (short)



//prototype functions
char ModBus(unsigned char NumByte);//подпрограмма обработки посылки
void StartTrans0(void);//начинаем передачу
int GetCRC16(unsigned char *buf,  char bufsize);
char ErrorMessage(char Error);
void do_modbus(void);
void TIM3_IRQHandler(void);
void USART1_IRQHandler(void);

//Таймер 4 - формирование посылки modbus
#define clr_int_tim_mb TIM_ClearITPendingBit(TIM3, TIM_IT_Update)
#define disable_tim_mb TIM_Cmd(TIM3, DISABLE)
#define enable_tim_mb TIM_Cmd(TIM3, ENABLE)
#define restart_tim_mb TIM3->CNT=0x00


//Разрешение\запрещение прерываний usart, очистка признаков
#define mb_receive_disable USART_ITConfig(USART1, USART_IT_RXNE, DISABLE)
#define mb_receive_enable USART_ITConfig(USART1, USART_IT_RXNE, ENABLE)
#define mb_trans_disable USART_ITConfig(USART1, USART_IT_TC, DISABLE)
#define mb_trans_enable USART_ITConfig(USART1, USART_IT_TC, ENABLE)
#define clr_int_txend USART_ClearITPendingBit(USART1, USART_IT_TC)
#define clr_int_rx USART_ClearITPendingBit(USART1, USART_IT_RXNE)



#endif /* MODBUS_H_ */
