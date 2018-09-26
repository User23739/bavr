
/*-----Инклуды--------- */

#include "main.h"




//Измерения производятся с переодом 1 мс.


/*-----------Данные-------------------------------------------------------*/
/*-----Сигнатуры функций--------------------------------------------------*/





//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[20] = {'\0'};  // буфер для передачи данных, примитивный



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
extern uint32_t buff_chanA1[];
extern uint32_t buff_chanB1[];
extern int a1;
extern int b1;

short flag_aktiv_channel = 0;				// 1 - канал А; 0 - оба выключенны; 2 - канал В.
short flag_status_chann_A = 0;				// состояние канала А;  0 - канал норм, 1 - канал не в норм.
short flag_status_chann_B = 0;				// состояние канала B;  0 - канал норм, 1 - канал не в норм.
short flag_switch_A = 0;		    		// 0 - вкл; 1 - откл
short flag_switch_B = 0;		    		// 0 - вкл; 1 - откл

//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //Полный период Резерв на всякий случай     */

uint32_t SIN_A_ref_up[] = {2048, 2152, 2255, 2356, 2455, 2552, 2646, 2737, 2823, 2904, 2980,
		3050, 3114, 3171, 3222, 3265, 3301, 3329, 3349, 3361, 3365, 3361, 3349, 3329, 3301,
		3265, 3222, 3171, 3114, 3050, 2980, 2904, 2823, 2737, 2646, 2552, 2455, 2356, 2255, 2152, 2048};			// идеал положительной полуволны синусоиды

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




void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
    	Aver();						//Вызвываем функцию усреднения
    	ProcessTimers();			//Вызываем функцию увеличения значений таймера
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

/*
 Поиск 0 и синхронизация */
/*	if (flag_sinch_chan_A == 0)	{
		if((SIN_A_ref_up[0]-shift10) < vol[0] && (SIN_A_ref_up[0]+shift10) > vol[0]) {
			flag_sinch_chan_A =1;
			k = 0;
		}
		else{
			flag_sinch_chan_A = 0;
		}
	}
*/


/*Функция синхронизации */
//Инициализация функции синхронизации
short count_work_err = 0;  //счетчик ошибок синхронизации
short err_flag_sinch = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
short flag_sinch_chan_A = 0;					// 0-нет синхронизации; 1-есть синхронизация
short flag_sinch_chan_B = 0;					// 0-нет синхронизации; 1-есть синхронизация


void InitSynchA(){
	count_work_err = 0;
	err_flag_sinch = 0;

}
//Процесс функции инициализации
void SynchA (double *vol){

	if (flag_sinch_chan_A == 0){
		if ((REF_MIN < vol[0]) && (vol[0] > REF_MAX)){
			flag_sinch_chan_A = 1;
			 err_flag_sinch = 0;
			StartGTimer(GTIMER4);
		}
		else{
			count_work_err++;
			if(count_work_err < COUN_SINCH_ERR) err_flag_sinch = 1;
		}
	}


}
/*Функция определения квадранта синуса*/

void SinQuadrant(){







	/*a1l = a1-1;
		if ((k == 0) && (flag_sinch_chan_A == 1)){

			if (buff_chanA1[a1l] > SIN_A_ref_up[0]){
					flag_mov_sin_A = 1;
			}
			else{
					flag_mov_sin_A = 0;
			}
		}*/
}

/*Функция расчета частоты*/
void Freq(){

}


/*Функция расчета среднеквадратичного значения*/
void TrueRMS(){

}


int k = 0;                                  // счетчик измерений от ноля
int a1l=0;									// переменная для хранения пред идущего указателя буфера

short flag_mov_sin_A = 0;						    // 0 -идем вверх, 1 -идем вниз
short int flag_channel_A[3]={0};				//[0] - флаг состояния АА  0 - хорошо; 1 - плохая;
												//[1] - флаг состояния AB
												//[2] - флаг состояния AC

//---------- функция сравнения синуса канала A-----------------------------------------------------------
/// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.



void sin_compar_A(double  *vol){
	if (k == 10) k = 0;



/*Сравнение синусоиды положительная полуволна*/
	if ((flag_mov_sin_A == 0) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref_up[k]-shift10) < vol[0] && (SIN_A_ref_up[k]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		else{
			flag_channel_A[0] = 1;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		//---------------------------------------------AB------------------------------------------------------
		if((SIN_B_ref_up[k]-shift10) < vol[1] && (SIN_B_ref_up[k]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//---------------------------------------------AC------------------------------------------------------
		if((SIN_C_ref_up[k]-shift10) < vol[2] && (SIN_C_ref_up[k]+shift10) > vol[2]) {
			flag_channel_A[2] = 0;
		}
		else{
			flag_channel_A[2] = 1;
		}
	}
/*Сравнение синусоиды отрицательная полуволна*/
	if ((flag_mov_sin_A == 1) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref_dw[k]-shift10) < vol[0] && (SIN_A_ref_dw[k]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		else{
			flag_channel_A[0] = 1;
			k++;
			if (k == 10) flag_sinch_chan_A = 0;
		}
		//-----------------------------------------------AB----------------------------------------------------
		if((SIN_B_ref_dw[k]-shift10) < vol[1] && (SIN_B_ref_dw[k]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
		}
		else{
			flag_channel_A[1] = 1;
		}
		//-----------------------------------------------AC----------------------------------------------------
		if((SIN_C_ref_dw[k]-shift10) < vol[2] && (SIN_C_ref_dw[k]+shift10) > vol[2]){
			flag_channel_A[2] = 0;
		}
		else{
			flag_channel_A[2] = 1;
		}
	}

}
//функция сравнения синусоиды канала B

// предидущие значения
int kb = 0;                                  // счетчик измерений от ноля

short flag_mov_sin_B = 0;						    // 0 -идем вверх, 1 -идем вниз
short int flag_channel_B[3]={0};				//[0] - флаг состояния BА   0 - хорошо; 1 - плохая;
												//[1] - флаг состояния BB
												//[2] - флаг состояния BC



//--------- функция сравнения синуса канала A------------------------------------------------------------
void sin_compar_B(double *vol){  /// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.
	//смотрим счетчик досчитал ли до 10 значений
	if (kb < 10){
		kb = 0;
		flag_sinch_chan_B =0;
	}

	after_B = befor_B;
	befor_B = vol[3];

	if (after_B !=0){		// сморим есть ли предидущие значения

		if (kb == 0){				// находили ли до этого момента ноль

			if((SIN_A_ref_up[kb]-shift10) < vol[3] && (SIN_A_ref_up[kb]+shift10) > vol[3]){   //Детектирование 0 и переход дальше
				flag_sinch_chan_B =1;
						// необходимо определить направление синусоиды.
					if (after_B > SIN_A_ref_up[kb])	{
							flag_mov_sin_B = 1;
							kb++;
						}
					else{
							flag_mov_sin_B = 0;
							kb++;
						}
				}
			else{
				flag_sinch_chan_B =0;
			}
		}
		else{		// если уже был найден ноль
			// надо еще выбрать напорвление движения по синусоиде
			if (flag_mov_sin_B == 0){  //тут смотрим движение движение и как в него попасть повторно
				//----------------------------------------------BA1-----------------------------------------------------
				if((SIN_A_ref_up[k]-shift10) < vol[3] && (SIN_A_ref_up[k]+shift10) > vol[3]){     // сравниваем синусоиду
					flag_channel_B[0] = 0;
					kb++;
				}
				else{
					flag_channel_B[0] = 1;
					kb++;
				}
				//---------------------------------------------BB1------------------------------------------------------
				if((SIN_B_ref_up[k]-shift10) < vol[4] && (SIN_B_ref_up[k]+shift10) > vol[4]){     // сравниваем синусоиду
					flag_channel_B[1] = 0;
				}
				else{
					flag_channel_B[1] = 1;
				}
				//---------------------------------------------BC1------------------------------------------------------
				if((SIN_C_ref_up[k]-shift10) < vol[5] && (SIN_C_ref_up[k]+shift10) > vol[5]){     // сравниваем синусоиду
					flag_channel_B[2] = 0;
				}
				else{
					flag_channel_B[2] = 1;
				}
			}
			else{
				//----------------------------------------------BA1-----------------------------------------------------
				if((SIN_A_ref_dw[k]-shift10) < vol[3] && (SIN_A_ref_dw[k]+shift10) > vol[3]){
					flag_channel_B[0] = 0;
					k++;
				}
				else{
					flag_channel_B[0] = 1;
					k++;
				}
				//-----------------------------------------------BB1----------------------------------------------------
				if((SIN_B_ref_dw[k]-shift10) < vol[4] && (SIN_B_ref_dw[k]+shift10) > vol[4]){
					flag_channel_B[1] = 0;
				}
				else{
					flag_channel_B[1] = 1;
				}
				//-----------------------------------------------BC1----------------------------------------------------
				if((SIN_C_ref_dw[k]-shift10) < vol[5] && (SIN_C_ref_dw[k]+shift10) > vol[5]){
					flag_channel_B[2] = 0;
				}
				else{
					flag_channel_B[2] = 1;
				}
			}
		}
	}
}



int main(void){



// инициализация переферии

	SYSTEM_Init();
	//RELAY_Init();
	ADC_DMA_Init();
	TIMER_Init();
	RS232_Init();
	//InitGPIO();
	InitGTimers();  // инициализируем глобальные таймеры
	//InitKey();		//инициализация каналов переключения (отключение)
	InitSynchA();

	while(1){

				/*ADC_SoftwareStartConv(ADC1);
	            adc_value = ADC_GetConversionValue(ADC1);
	            sprintf(buffer, "%d\r\n", adc_value);
	            USARTSend(buffer, sizeof(buffer));*/

		//channel_status();
		//switch_channel();
		//channel_A_ON();
		//channel_B_ON();
		//channel_A_OFF();
		//channel_B_OFF();
		//ButControl();
	}
}
