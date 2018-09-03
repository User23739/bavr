
/*-----Инклуды--------- */

/*-----Стандартные библиотеки-----------*/
#include <stdint.h>
#include <stdio.h>
#include <time.h>
//#include <math.h">

/*----CMSIS и SPL----------*/
#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"

/*----Логика----------*/
#include "system.h"
#include "relay.h"
#include "adc_dma_mesur.h"
#include "rs232.h"
#include "tim_mesure.h"
#include "timers.h"


#define KEY_DELAY 45

//Измерения производятся с переодом 1 мс.


extern uint32_t ADCBuffer[]; 		//переменная значений из АЦП
extern short flag_priori_chann_manual;	//переменная приоритека танала
/*-----------Данные-------------------------------------------------------*/
/*-----Сигнатуры функций--------------------------------------------------*/
void send_buffer(uint32_t *vol_tmp_chan);
void USARTSend(const unsigned char *pucBuffer);
void Aver(void);
void sin_compar_A(uint32_t *vol);
void sin_compar_B(uint32_t *vol);
void channel_status(void);
void switch_channel(void);
void channel_A_ON(void);
void channel_B_ON(void);
void channel_A_OFF(void);
void channel_B_OFF(void);
void InitKey(void);



//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[10] = {'\0'};  // буфер для передачи данных, примитивный
char BufTest[500] = {'\0'};			//буфер состояния флагов

void SendBuff(void){

}


void USARTSend(const unsigned char *pucBuffer){
	while (*pucBuffer)
	{
		USART_SendData(USART1, *pucBuffer++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
		}
	}
}


//--глобальные переменные



short flag_aktiv_channel = 0;				// 1 - канал А; 0 - оба выключенны; 2 - канал В.
short flag_status_chann_A = 0;				// состояние канала А;  0 - канал норм, 1 - канал не в норм.
short flag_status_chann_B = 0;				// состояние канала B;  0 - канал норм, 1 - канал не в норм.
short flag_switch_A = 0;		    		// 0 - вкл; 1 - откл
short flag_switch_B = 0;		    		// 0 - вкл; 1 - откл

//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //Полный период Резерв на всякий случай     */

uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758, 2390 };			// идеал положительной полуволны синусоиды
uint32_t SIN_A_ref_dw[] = {1983, 1577, 1209, 918, 731, 666, 731, 918, 1209, 1577 };			// идеал отрицательной полуволны синусоиды

uint32_t SIN_C_ref_up[] = {3124, 2865, 2520, 2122, 1710, 1325, 1005, 781, 674, 695 };			// идеал положительной полуволны синусоиды A
uint32_t SIN_C_ref_dw[] = {843, 1102, 1447, 1845, 2257, 2642, 2962, 3186, 3293, 3272 };			// идеал отрицательной полуволны синусоиды A

uint32_t SIN_B_ref_up[] = {843,	695, 674, 780, 1005, 1325, 1710, 2121, 2519, 2865 };			// идеал положительной полуволны синусоиды A
uint32_t SIN_B_ref_dw[] = {3124, 3272, 3293, 3187, 2962, 2642, 2257, 1846, 1448, 1102 };			// идеал отрицательной полуволны синусоиды A


//переменные допуска по синусоиде 10%, 15%, 20%

uint32_t shift10 = 132;
uint32_t shift15 = 198;
uint32_t shift20 = 264;


//-------------------------

//переменные для хранения текущих значений измерения

//Обработчик таймера
//volatile uint32_t vol_tmp_chan[7] = {0}; // переменная куда помещаются измеренные данные
										// с АЦП
										// [0]-КАНАЛ А ФАЗА 1
										// [1]-КАНАЛ А ФАЗА 2
										// [2]-КАНАЛ А ФАЗА 3
										// [3]-КАНАЛ В ФАЗА 1
										// [4]-КАНАЛ В ФАЗА 2
										// [5]-КАНАЛ В ФАЗА 3
										// [6]-КАНАЛ С ФАЗА 1
volatile uint32_t aver_tmp_chan[7] = {0}; // переменная куда помещаются измеренные данные
										// с АЦП
										// [0]-КАНАЛ А ФАЗА 1
										// [1]-КАНАЛ А ФАЗА 2
										// [2]-КАНАЛ А ФАЗА 3
										// [3]-КАНАЛ В ФАЗА 1
										// [4]-КАНАЛ В ФАЗА 2
										// [5]-КАНАЛ В ФАЗА 3
										// [6]-КАНАЛ С ФАЗА 1



void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
    	Aver();						//Вызвываем функцию усреднения
    	ProcessTimers();			//Вызываем функцию увеличения значений таймера
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    }


}

// переменные  буфера
// буфер кольцево для хранения данных измерения
uint32_t buff_chanA1[201] = {0};
uint32_t buff_chanA2[201] = {0};
uint32_t buff_chanA3[201] = {0};

uint32_t buff_chanB1[201] = {0};
uint32_t buff_chanB2[201] = {0};
uint32_t buff_chanB3[201] = {0};

uint32_t buff_chanC1[201] = {0};

//указатель кольцевого буфера
int a1, a2, a3 = 0;			//указатели буфера канала А
int b1, b2, b3 = 0;			//указатели буфера канала B
int c1 = 0;					//указатели буфера канала С


void send_buffer(uint32_t *vol_tmp_chan){

	if(a1 == 201 ) a1 = 0;
	buff_chanA1[a1] = vol_tmp_chan[0];
	a1++;
	if(a2 == 201 ) a2 = 0;
	buff_chanA2[a2] = vol_tmp_chan[1];
	a2++;
	if(a3 == 201 ) a3 = 0;
	buff_chanA3[a3] = vol_tmp_chan[2];
	a3++;
	if(b1 == 201 ) b1 = 0;
	buff_chanB1[b1] = vol_tmp_chan[3];
	b1++;
	if(b2 == 201 ) b2 = 0;
	buff_chanB2[b2] = vol_tmp_chan[4];
	b2++;
	if(b3 == 201 ) b3 = 0;
	buff_chanB3[b3] = vol_tmp_chan[5];
	b3++;
	if(c1 == 201 ) c1 = 0;
	buff_chanC1[c1] = vol_tmp_chan[6];
	c1++;
}




/*void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	    {

			SWICH_FLAG = 1;

	        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	    }

}*/




//функция усреднения и фильтация фходных значений
int count_mes = 0;


void Aver(void){

	if (count_mes != 3 ){

	    for (int i=0; i<7; i++){
	    	aver_tmp_chan[i] += ADCBuffer[i];
			}
	    count_mes++;
	}
	else{
		for (int i=0; i<7; i++){
			aver_tmp_chan[i] = aver_tmp_chan[i]/3;
			}
		//тут вызываем функцию пересылки данных в буфер
		send_buffer(aver_tmp_chan);
		count_mes = 0;
		sin_compar_A(aver_tmp_chan);	//Вызываем функцию сравнения канала А
		sin_compar_B(aver_tmp_chan);	//Вызываем функцию сравнения канала B

		for (int i=0; i<7; i++) aver_tmp_chan[i] = 0;
	}


}



//-------------------Функция сравнения синусоиды полученной с опорной---------------------------------------------------
//Переменные для функции сравнения
/*volatile uint32_t vol[0] = 0;
volatile uint32_t vol_tmp_chnA2 = 0;
volatile uint32_t vol_tmp_chnA3 = 0;

volatile uint32_t vol_tmp_chnB1 = 0;
volatile uint32_t vol_tmp_chnB2 = 0;
volatile uint32_t vol_tmp_chnB3 = 0;

volatile uint32_t vol_tmp_chnC1 = 0;*/

//счетчики измерений


//данные идеальных синусоид с периодом измерения 1 мс


volatile uint32_t befor = 0;

volatile uint32_t after = 0;								// предидущие значения
uint8_t k = 0;                                  // счетчик измерений от ноля

short flag_mov_sin_A = 0;						    // 0 -идем вверх, 1 -идем вниз

int text_sin_add_ok = 100	;				    // посолительная волна все ок
int text_sin_add_not_ok = 101	;				// посолительная волна все не ок

int text_sin_div_ok = 200	;				    // отрицательная волна все ок
int text_sin_div_not_ok = 201	;				// отрицательна волна все не ок

short int flag_channel_A[3]={0};				//[0] - флаг состояния АА  0 - хорошо; 1 - плохая;
												//[1] - флаг состояния AB
												//[2] - флаг состояния AC

//---------- функция сравнения синуса канала A-----------------------------------------------------------
void sin_compar_A(uint32_t *vol){  /// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.
	//смотрим счетчик досчитал ли до 10 значений
	if (k == 9)
	{
		k = 0;

	}

	after = befor;
	befor = vol[0];

	if (after !=0)		// сморим есть ли предидущие значения
	{
		if (k == 0)				// находили ли до этого момента ноль
		{
			if((SIN_A_ref_up[k]-shift10) < vol[0] && (SIN_A_ref_up[k]+shift10) > vol[0])    //Детектирование 0 и переход дальше
				{
						// необходимо определить направление синусоиды.
					if (after > SIN_A_ref_up[k])
						{
							flag_mov_sin_A = 1;
							k++;
						}
					else
						{
							flag_mov_sin_A = 0;
							k++;
						}
				}
		}
		else		// если уже был найден ноль
		{
			// надо еще выбрать напорвление движения по синусоиде
			if (flag_mov_sin_A == 0)  //тут смотрим движение движение и как в него попасть повторно
			{
				//----------------------------------------------AA-----------------------------------------------------

				if((SIN_A_ref_up[k]-shift10) < vol[0] && (SIN_A_ref_up[k]+shift10) > vol[0])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					flag_channel_A[0] = 0;
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					k++;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					flag_channel_A[0] = 1;
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					k++;
				}

				//---------------------------------------------AB------------------------------------------------------
				if((SIN_B_ref_up[k]-shift10) < vol[1] && (SIN_B_ref_up[k]+shift10) > vol[1])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					flag_channel_A[1] = 0;

				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					flag_channel_A[1] = 1;

				}
				//---------------------------------------------AC------------------------------------------------------
				if((SIN_C_ref_up[k]-shift10) < vol[2] && (SIN_C_ref_up[k]+shift10) > vol[2])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					flag_channel_A[2] = 0;

				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					flag_channel_A[3] = 1;

				}

			}
			else
			{
				//----------------------------------------------AA-----------------------------------------------------

				if((SIN_A_ref_dw[k]-shift10) < vol[0] && (SIN_A_ref_dw[k]+shift10) > vol[0])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_A[0] = 0;
					k++;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_A[0] = 1;
					k++;
				}

				//-----------------------------------------------AB----------------------------------------------------

				if((SIN_B_ref_dw[k]-shift10) < vol[1] && (SIN_B_ref_dw[k]+shift10) > vol[1])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_A[1] = 0;

				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_A[1] = 1;

				}
				//-----------------------------------------------AC----------------------------------------------------

				if((SIN_C_ref_dw[k]-shift10) < vol[2] && (SIN_C_ref_dw[k]+shift10) > vol[2])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_A[2] = 0;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_A[2] = 1;

				}

			}
		}


	}




}
//функция сравнения синусоиды канала B

volatile uint32_t befor_B = 0;

volatile uint32_t after_B = 0;								// предидущие значения
uint8_t kb = 0;                                  // счетчик измерений от ноля

short flag_mov_sin_B = 0;						    // 0 -идем вверх, 1 -идем вниз

short int flag_channel_B[3]={0};				//[0] - флаг состояния BА   0 - хорошо; 1 - плохая;
												//[1] - флаг состояния BB
												//[2] - флаг состояния BC



//--------- функция сравнения синуса канала A------------------------------------------------------------
void sin_compar_B(uint32_t *vol){  /// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.
	//смотрим счетчик досчитал ли до 10 значений
	if (kb == 9)
	{
		kb = 0;

	}

	after_B = befor_B;
	befor_B = vol[3];

	if (after_B !=0)		// сморим есть ли предидущие значения
	{
		if (kb == 0)				// находили ли до этого момента ноль
		{
			if((SIN_A_ref_up[kb]-shift10) < vol[3] && (SIN_A_ref_up[kb]+shift10) > vol[3])    //Детектирование 0 и переход дальше
				{
						// необходимо определить направление синусоиды.
					if (after_B > SIN_A_ref_up[kb])
						{
							flag_mov_sin_B = 1;
							kb++;
						}
					else
						{
							flag_mov_sin_B = 0;
							kb++;
						}
				}
		}
		else		// если уже был найден ноль
		{
			// надо еще выбрать напорвление движения по синусоиде
			if (flag_mov_sin_B == 0)  //тут смотрим движение движение и как в него попасть повторно
			{

				//----------------------------------------------BA1-----------------------------------------------------

				if((SIN_A_ref_up[k]-shift10) < vol[3] && (SIN_A_ref_up[k]+shift10) > vol[3])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					flag_channel_B[0] = 0;
					kb++;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					flag_channel_B[0] = 1;
					kb++;
				}

				//---------------------------------------------BB1------------------------------------------------------
				if((SIN_B_ref_up[k]-shift10) < vol[4] && (SIN_B_ref_up[k]+shift10) > vol[4])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					flag_channel_B[1] = 0;

				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					flag_channel_B[1] = 1;

				}
				//---------------------------------------------BC1------------------------------------------------------
				if((SIN_C_ref_up[k]-shift10) < vol[5] && (SIN_C_ref_up[k]+shift10) > vol[5])     // сравниваем синусоиду
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n",  text_sin_add_ok);
					flag_channel_B[2] = 0;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_add_not_ok);
					flag_channel_B[2] = 1;
				}


			}
			else
			{

				//----------------------------------------------BA1-----------------------------------------------------

				if((SIN_A_ref_dw[k]-shift10) < vol[3] && (SIN_A_ref_dw[k]+shift10) > vol[3])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_B[0] = 0;
					k++;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_B[0] = 1;
					k++;
				}

				//-----------------------------------------------BB1----------------------------------------------------

				if((SIN_B_ref_dw[k]-shift10) < vol[4] && (SIN_B_ref_dw[k]+shift10) > vol[4])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_B[1] = 0;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_B[1] = 1;
				}
				//-----------------------------------------------BC1----------------------------------------------------

				if((SIN_C_ref_dw[k]-shift10) < vol[5] && (SIN_C_ref_dw[k]+shift10) > vol[5])
				{
					//выставляем флаг ок
					//sprintf(buffer, "%d\r\n", text_sin_div_ok);
					flag_channel_B[2] = 0;
				}
				else
				{
					// по идее нужно смотреть ходябы пару измерений, что они не в норме, но это на будущее
					//выставляем флаг не ок, и даем команду на переключение
					//sprintf(buffer, "%d\r\n", text_sin_div_not_ok);
					flag_channel_B[2] = 1;
				}
			}
		}


	}




}

//----------функция переключения------------------------------------------------------------------------
//судить о качестве измерительного канала будем судить по 3 последовательным отказам по любой фазе
int count_err_A[4] = {0};			//[0]- общий на канал; [1]- AA; [2]-AB; [3]-AC;
int count_err_B[4] = {0};			//[0]- общий на канал; [1]- BA; [2]-BB; [3]-BC;

void channel_status(void){

	count_err_A[0]++;
	if ((flag_channel_A[0]==0)&&(flag_channel_A[1]==0)&&(flag_channel_A[2]==0)){
		flag_status_chann_A = 0;
	}
	else{
		if (flag_channel_A[0]==1) count_err_A[1] += 1;
		if (flag_channel_A[1]==1) count_err_A[2] += 1;
		if (flag_channel_A[2]==1) count_err_A[3] += 1;
		if ((count_err_A[0] == 3)&&((count_err_A[1] == 3)||(count_err_A[2] == 3)||(count_err_A[3] == 3))) flag_status_chann_A = 1 ;
		if (count_err_A[0] == 3) for (int i=0; i<4; i++)count_err_A[i] = 0;
	}

	count_err_B[0]++;
	if ((flag_channel_B[0]==0)&&(flag_channel_B[1]==0)&&(flag_channel_B[2]==0)){
		flag_status_chann_B = 0;
	}
	else{
		if (flag_channel_B[0]==1) count_err_B[1] += 1;
		if (flag_channel_B[1]==1) count_err_B[2] += 1;
		if (flag_channel_B[2]==1) count_err_B[3] += 1;
		if ((count_err_B[0] == 3)&&((count_err_B[1] == 3)||(count_err_B[2] == 3)||(count_err_B[3] == 3))) flag_status_chann_B = 1 ;
		if (count_err_B[0] == 3) for (int i=0; i<4; i++)count_err_B[i] = 0;
	}


}

//--------функция принятия решения о переключении-------------------------------------------------------

void switch_channel(void){
	if((flag_status_chann_A == 0)&&(flag_status_chann_B == 0)){
		if(flag_priori_chann_manual == 0){    //если в ручную активирован канал А
			switch(flag_aktiv_channel){
				case 0:
					//выставляем флаг на включение канала А
					flag_switch_A = 0;

					break;
				case 1:
					//ничего не делаем
					break;
				case 2:
					//выставляем флаг на включение канала А
					flag_switch_A = 0;

					break;
			}
		}
		else{  								//если в ручную активирован канал В
			switch(flag_aktiv_channel){
				case 0:
					//выставляем флаг на включение канала B
					flag_switch_B = 0;

					break;
				case 1:
					//выставляем флаг на включение канала B
					flag_switch_B = 0;

					break;
				case 2:
					//ничего не деалем
					break;
				}
		}
	}
	else if((flag_status_chann_A == 1)&&(flag_status_chann_B == 1)){
		flag_switch_A = 1;		//флаг на отключение канала
		flag_switch_B = 1;		//флаг на отключение канала
	}
	else{
		if (flag_status_chann_A == 1){
			flag_switch_A = 1;		//флаг на отключение канала
			flag_switch_B = 0;		//флаг на включение канала


		}
		if (flag_status_chann_B == 1){
			flag_switch_B = 1;		//флаг на отключение канала
			flag_switch_A = 0;		//флаг на включение канала

		}

	}

}

//-------функция включения канала А---------------------------------------------------------------------

void channel_A_ON(void){
	unsigned int Timer2 = 0;
	unsigned int Timer3 = 0;
	if (flag_switch_A == 0){       //флаг с условием для включения канала А
		switch(flag_aktiv_channel){
			case 0:
				Timer3 = GetGTimer(GTIMER3);
				if(Timer3 >KEY_DELAY){
				/*Включаемм транзисторы */
					//1
					GPIO_SetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
					//2
					GPIO_SetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
					//3
					GPIO_SetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
					flag_aktiv_channel = 1;

				/*Включаем реле*/
					//1
					GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
					GPIO_SetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
					//2
					GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
					GPIO_SetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
					//3
					GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
					GPIO_SetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
					StopGTimer(GTIMER3);
					Timer3 = 0;
				}

				break;
			case 1:
				/*тут ничего не делаем и просто выходим*/
				break;
			case 2:
					Timer2 = GetGTimer(GTIMER2);
					if(Timer2 > KEY_DELAY){
						/*Включаемм транзисторы */
						//1
						GPIO_SetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
						//2
						GPIO_SetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
						//3
						GPIO_SetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
						flag_aktiv_channel = 1;

					/*Включаем реле*/
						//1
						GPIO_ResetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
						GPIO_SetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
						//2
						GPIO_ResetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
						GPIO_SetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
						//3
						GPIO_ResetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
						GPIO_SetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
						StopGTimer(GTIMER2);
						Timer2 = 0;

					}

				break;
		}
	}
	else{
		/*тут возможна альтернативная реализация*/
	}


}

//-------функция включения канала B---------------------------------------------------------------------

void channel_B_ON(void){
	unsigned int Timer1 = 0;
	unsigned int Timer3 = 0;
	if (flag_switch_B == 0){
			switch(flag_aktiv_channel){
				case 0:
					Timer3 = GetGTimer(GTIMER3);
					if(Timer3 >KEY_DELAY){
					/*Включаемм транзисторы */
						//4
						GPIO_SetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
						//5
						GPIO_SetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
						//6
						GPIO_SetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
						flag_aktiv_channel = 2;


					/*Включаем реле*/
						//4
						GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
						GPIO_SetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
						//5
						GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
						GPIO_SetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
						//6
						GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
						GPIO_SetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
						StopGTimer(GTIMER3);
						Timer3 = 0;
					}


					break;
				case 1:
						Timer1 = GetGTimer(GTIMER1);
						if(Timer1 >KEY_DELAY){
							/*Включаемм транзисторы */
							//4
							GPIO_SetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
							//5
							GPIO_SetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
							//6
							GPIO_SetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
							flag_aktiv_channel = 2;

						/*Включаем реле*/
							//4
							GPIO_ResetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
							GPIO_SetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
							//5
							GPIO_ResetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
							GPIO_SetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
							//6
							GPIO_ResetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
							GPIO_SetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
							StopGTimer(GTIMER1);
							Timer1 = 0;
						}

						break;
				case 2:
					/*тут ничего не делаем*/
					   break;
			}
		}
		else{
				/*тут возможна альтернативная реализация*/
		}



}

//-------функция отключения канала А---------------------------------------------------------------------

void channel_A_OFF(void){
	if (flag_switch_A == 1){
	/*Отключаем транзисторы */
		//1
		GPIO_ResetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
		//2
		GPIO_ResetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
		//3
		GPIO_ResetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
		/*Отключаем 1  реле */
		GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
		GPIO_SetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
		/*Отключаем 2 реле */
		GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
		GPIO_SetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
		/*Отключаем 3  реле */
		GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
		GPIO_SetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
		/*тут даем команду на старт таймера отсрочки времени отключения реле 15 мс*/
		StartGTimer(GTIMER1);
	}




}

//-------функция отключения канала B---------------------------------------------------------------------

void channel_B_OFF(void){
	if (flag_switch_B == 1){
	/*Отключаем транзисторы */
	//4
	GPIO_ResetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
	//5
	GPIO_ResetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
	//6
	GPIO_ResetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
	/*Отключаем 4  реле */
	GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
	GPIO_SetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
	/*Отключаем 5  реле */
	GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
	GPIO_SetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
	/*Отключаем 6  реле */
	GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
	GPIO_SetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
	/*тут даем команду на старт таймера отсрочки времени отключения реле 15 мс*/
	StartGTimer(GTIMER2);
	}

}
//------функция инициализации силовых ключей--------------------------------------------------------------

void InitKey(void){
	/*Отключаем транзисторы */
	//1
	GPIO_ResetBits(RELAY_1_PORT_IGBT, RELAY_1_PIN_IGBT);
	//2
	GPIO_ResetBits(RELAY_2_PORT_IGBT, RELAY_2_PIN_IGBT);
	//3
	GPIO_ResetBits(RELAY_3_PORT_IGBT, RELAY_3_PIN_IGBT);
	//4
	GPIO_ResetBits(RELAY_4_PORT_IGBT, RELAY_4_PIN_IGBT);
	//5
	GPIO_ResetBits(RELAY_5_PORT_IGBT, RELAY_5_PIN_IGBT);
	//6
	GPIO_ResetBits(RELAY_6_PORT_IGBT, RELAY_6_PIN_IGBT);
	/*Отключаем 1  реле */
	GPIO_ResetBits(RELAY_1_PORT_ON, RELAY_1_PIN_ON);
	GPIO_SetBits(RELAY_1_PORT_OFF, RELAY_1_PIN_OFF);
	/*Отключаем 2 реле */
	GPIO_ResetBits(RELAY_2_PORT_ON, RELAY_2_PIN_ON);
	GPIO_SetBits(RELAY_2_PORT_OFF, RELAY_2_PIN_OFF);
	/*Отключаем 3  реле */
	GPIO_ResetBits(RELAY_3_PORT_ON, RELAY_3_PIN_ON);
	GPIO_SetBits(RELAY_3_PORT_OFF, RELAY_3_PIN_OFF);
	/*Отключаем 4  реле */
	GPIO_ResetBits(RELAY_4_PORT_ON, RELAY_4_PIN_ON);
	GPIO_SetBits(RELAY_4_PORT_OFF, RELAY_4_PIN_OFF);
	/*Отключаем 5  реле */
	GPIO_ResetBits(RELAY_5_PORT_ON, RELAY_5_PIN_ON);
	GPIO_SetBits(RELAY_5_PORT_OFF, RELAY_5_PIN_OFF);
	/*Отключаем 6  реле */
	GPIO_ResetBits(RELAY_6_PORT_ON, RELAY_6_PIN_ON);
	GPIO_SetBits(RELAY_6_PORT_OFF, RELAY_6_PIN_OFF);
	/*тут даем команду на старт таймера отсрочки времени отключения реле 15 мс*/
	StartGTimer(GTIMER3);
	flag_aktiv_channel = 0;			//выставляем флаг отключения обоих каналов
}


int main(void){



// инициализация переферии

	SYSTEM_Init();
	RELAY_Init();
	ADC_DMA_Init();
	TIMER_Init();
	RS232_Init();
	InitGPIO();
	InitGTimers();  // инициализируем глобальные таймеры
	InitKey();		//инициализация каналов переключения (отключение)

	while(1){
		channel_status();
		switch_channel();
		channel_A_ON();
		channel_B_ON();
		channel_A_OFF();
		channel_B_OFF();
		ButControl();






		//USARTSend(buffer);
		uint8_t i = 0;

				//uint32_t ADC_chan1[200] = {0};
				TIM_Cmd(TIM4, ENABLE);

				if (buff_chanA1[200] != 0){
					TIM_Cmd(TIM4, DISABLE);
					//sprintf(buffer, "%d\r\n", start);
					//USARTSend(buffer);

					for (i=0; i<201; i++){

							sprintf(buffer, "%d\r\n", buff_chanA1[i]);
							USARTSend(buffer);

						}

					for (i=0; i<201; i++) buff_chanA1[i] = 0;




				}

	}

}
