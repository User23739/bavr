
/*-----Инклуды--------- */

#include "main.h"

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

short flag_aktiv_channel = 0;				// 1 - канал А; 0 - оба выключенны; 2 - канал В.
short flag_status_chann_A = 0;				// состояние канала А;  0 - канал норм, 1 - канал не в норм.
short flag_status_chann_B = 0;				// состояние канала B;  0 - канал норм, 1 - канал не в норм.
short flag_switch_A = 0;		    		// 0 - вкл; 1 - откл
short flag_switch_B = 0;		    		// 0 - вкл; 1 - откл

short flag_mov_sin_A = 0;						    // 0 -идем вверх, 1 -идем вниз
short flag_mov_sin_B = 0;						    // 0 -идем вверх, 1 -идем вниз


//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //Полный период Резерв на всякий случай     */

double SIN_A_ref[] = {13,5, 66, 116,5, 164,5, 208,5, 247, 279,5, 305,5, 323,5, 333,5, 335,5,
						329,5, 315,5, 293,5, 264, 228,5, 187, 141, 91,5, 40};			// идеал положительной полуволны синусоиды

double SIN_B_ref[] = {-297,5,	-318,5, -331,5, -336, -333, -321, -301,5, -274,5, -241,
						-201,5, -157, -108,5, -57, -4,5, 49, 100, 149, 194, 234,5, 269,5};			// сдвиг по синусоиде В


double SIN_C_ref[] = {284,5, 253, 215, 172,5, 75, 22,5, -31, -83, -133, -179,5, -222, -258,5,
							-289, -312, -328, -335,5,  -335, -326, -309};						// сдвиг по синусоиде С


//переменные допуска по синусоиде 10%, 15%, 20%

double shift10 = 24;
double shift15 = 36;
double shift20 = 48;


//-------------------------
/*тестовый буфер флагов */
// буфер кольцево для хранения данных измерения
char buff_flag_[2000] = {0};




//указатель кольцевого буфера
int a1 = 0;



void send_buffer_flag(char *vol){

	if(a1 == 2001 ) a1 = 0;
	buff_flag_[a1] = vol;
	a1++;


}



void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
    	Aver();						//Вызвываем функцию усреднения
    	ProcessTimers();			//Вызываем функцию увеличения значений таймера
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}



/*Функция синхронизации */
//Инициализация функции синхронизации
short count_work_A_err = 0;  //счетчик ошибок синхронизации
short err_flag_sinch_A = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
short count_work_B_err = 0;  //счетчик ошибок синхронизации
short err_flag_sinch_B = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
short flag_sinch_chan_A = 0;					// 0-нет синхронизации; 1-есть синхронизация
short flag_sinch_chan_B = 0;					// 0-нет синхронизации; 1-есть синхронизация


void InitSynchA(){
	count_work_A_err = 0;
	err_flag_sinch_A = 0;
	count_work_B_err = 0;
	err_flag_sinch_B = 0;

}
//Процесс функции инициализации
void SynchA (double *vol){

	if (flag_sinch_chan_A == 0){
		if ((REF_MIN < vol[0]) && (vol[0] < REF_MAX)){
			flag_sinch_chan_A = 1;
			send_buffer_flag("err_flag_sinch_A = 0||1");
			err_flag_sinch_A = 0;
			StartGTimer(GTIMER4);
		}
		else{
			count_work_A_err++;
			if(count_work_A_err < COUN_SINCH_ERR) err_flag_sinch_A = 1;
		}
	}

	if (flag_sinch_chan_B == 0){
			if ((REF_MIN < vol[3]) && (vol[3] < REF_MAX)){
				flag_sinch_chan_B = 1;
				send_buffer_flag("err_flag_sinch_B = 0||2");
				err_flag_sinch_B = 0;
				StartGTimer(GTIMER5);
			}
			else{
				count_work_B_err++;
				if(count_work_B_err < COUN_SINCH_ERR) err_flag_sinch_B = 1;
			}
		}

}
/*Функция определения квадранта синуса*/

void SinQuadrant(int *x, int *y, double *buffA, double *buffB){
	int tmpA = *x;
	int tmpB = *y;
	if(flag_sinch_chan_A == 1){
		tmpA--;
		if(buffA[tmpA] > ZIRO){
			flag_mov_sin_A = 0;
			send_buffer_flag("flag_mov_sin_A = 0||3");
		}
		else{
			flag_mov_sin_A = 1;
			send_buffer_flag("flag_mov_sin_A = 1||4");
		}
	}
	if(flag_sinch_chan_B == 1){
		tmpB--;
		if(buffB[tmpB] > ZIRO){
			flag_mov_sin_B = 0;
			send_buffer_flag("flag_mov_sin_B = 0||5");
		}
		else{
			flag_mov_sin_B = 1;
			send_buffer_flag("flag_mov_sin_B = 1||6");
		}
	}

}



/*Функция расчета частоты*/
void Freq(){

}


/*Функция расчета среднеквадратичного значения*/
void TrueRMS(){

}


int k = 0;                                  // счетчик измерений от ноля
int k0 = 0;									// переменная для хранения пред идущего указателя буфера


short int flag_channel_A[3]={0};				//[0] - флаг состояния АА  0 - хорошо; 1 - плохая;
												//[1] - флаг состояния AB
												//[2] - флаг состояния AC

//---------- функция сравнения синуса канала A-----------------------------------------------------------
/// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.



void sin_compar_A(double  *vol){
	if (k0 == k) k = 0;
	if (k == 0){
		k = sizeof(SIN_A_ref)/sizeof(double);
		k0 = 0;
	}


/*Сравнение синусоиды положительная полуволна*/
	if ((flag_mov_sin_A == 0) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((SIN_A_ref[k0]-shift10) < vol[0] && (SIN_A_ref[k0]+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			send_buffer_flag("flag_channel_A[0] = 0||7");
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				send_buffer_flag("flag_sinch_chan_A = 0||8");
			}
		}
		else{
			flag_channel_A[0] = 1;
			send_buffer_flag("flag_channel_A[0] = 1||9");
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				send_buffer_flag("flag_sinch_chan_A = 0||10");
			}
		}
		//---------------------------------------------AB------------------------------------------------------
		if((SIN_B_ref[k0]-shift10) < vol[1] && (SIN_B_ref[k0]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
			send_buffer_flag("flag_channel_A[1] = 0||11");
		}
		else{
			flag_channel_A[1] = 1;
			send_buffer_flag("flag_channel_A[1] = 1||12");
		}
		//---------------------------------------------AC------------------------------------------------------
		if((SIN_C_ref[k0]-shift10) < vol[2] && (SIN_C_ref[k0]+shift10) > vol[2]) {
			flag_channel_A[2] = 0;
			send_buffer_flag("flag_channel_A[2] = 0||13");
		}
		else{
			flag_channel_A[2] = 1;
			send_buffer_flag("flag_channel_A[2] = 1||14");
		}
	}
/*Сравнение синусоиды отрицательная полуволна*/
	if ((flag_mov_sin_A == 1) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if(((SIN_A_ref[k0]*-1)-shift10) < vol[0] && ((SIN_A_ref[k0]*-1)+shift10) > vol[0]){
			flag_channel_A[0] = 0;
			send_buffer_flag("flag_channel_A[0] = 0||15");
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				send_buffer_flag("flag_sinch_chan_A = 0||16");
			}
		}
		else{
			flag_channel_A[0] = 1;
			k0++;
			if (k0 == k){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				send_buffer_flag("flag_sinch_chan_A = 0||17");
			}
		}
		//-----------------------------------------------AB----------------------------------------------------
		if(((SIN_B_ref[k0]*-1)-shift10) < vol[1] && ((SIN_A_ref[k0]*-1)+shift10) > vol[1]){
			flag_channel_A[1] = 0;
			send_buffer_flag("flag_channel_A[1] = 0||18");
		}
		else{
			flag_channel_A[1] = 1;
			send_buffer_flag("flag_channel_A[1] = 1||19");
		}
		//-----------------------------------------------AC----------------------------------------------------
		if(((SIN_C_ref[k0]*-1)-shift10) < vol[2] && ((SIN_C_ref[k0]*-1)+shift10) > vol[2]){
			flag_channel_A[2] = 0;
			send_buffer_flag("flag_channel_A[2] = 0||20");
		}
		else{
			flag_channel_A[2] = 1;
			send_buffer_flag("flag_channel_A[2] = 1||21");
		}
	}

}
//функция сравнения синусоиды канала B

// предидущие значения
int kb = 0;                                  // счетчик измерений от ноля
int kb0 = 0;

short int flag_channel_B[3]={0};				//[0] - флаг состояния BА   0 - хорошо; 1 - плохая;
												//[1] - флаг состояния BB
												//[2] - флаг состояния BC



//--------- функция сравнения синуса канала A------------------------------------------------------------
void sin_compar_B(double  *vol){
	if (kb0 == kb) kb = 0;
	if (kb == 0){
		kb = sizeof(SIN_A_ref)/sizeof(double);
		kb0 = 0;
	}


/*Сравнение синусоиды положительная полуволна*/
	if ((flag_mov_sin_B == 0) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if((SIN_A_ref[kb0]-shift10) < vol[3] && (SIN_A_ref[kb0]+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			send_buffer_flag("flag_channel_B[0] = 0||22");
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag("flag_sinch_chan_B = 0||23");
			}
		}
		else{
			flag_channel_B[0] = 1;
			send_buffer_flag("flag_channel_B[0] = 1||24");
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag("flag_sinch_chan_B = 0||25");
			}
		}
		//---------------------------------------------BB------------------------------------------------------
		if((SIN_B_ref[kb0]-shift10) < vol[4] && (SIN_B_ref[kb0]+shift10) > vol[4]){
			flag_channel_B[1] = 0;
			send_buffer_flag("flag_channel_B[1] = 0||26");
		}
		else{
			flag_channel_B[1] = 1;
			send_buffer_flag("flag_channel_B[1] = 1||27");
		}
		//---------------------------------------------BC------------------------------------------------------
		if((SIN_C_ref[kb0]-shift10) < vol[5] && (SIN_C_ref[kb0]+shift10) > vol[5]) {
			flag_channel_B[2] = 0;
			send_buffer_flag("flag_channel_B[2] = 0||28");
		}
		else{
			flag_channel_B[2] = 1;
			send_buffer_flag("flag_channel_B[2] = 1||29");
		}
	}
/*Сравнение синусоиды отрицательная полуволна*/
	if ((flag_mov_sin_B == 1) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if(((SIN_A_ref[kb0]*-1)-shift10) < vol[3] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			send_buffer_flag("flag_channel_B[0] = 0||30");
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag("flag_sinch_chan_B = 0||31");
			}
		}
		else{
			flag_channel_B[0] = 1;
			send_buffer_flag("flag_channel_B[0] = 1||32");
			kb0++;
			if (kb0 == kb){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag("flag_sinch_chan_B = 0||33");
			}
		}
		//-----------------------------------------------BB----------------------------------------------------
		if(((SIN_B_ref[kb0]*-1)-shift10) < vol[4] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[4]){
			flag_channel_B[1] = 0;
			send_buffer_flag("flag_channel_B[1] = 0||34");
		}
		else{
			flag_channel_B[1] = 1;
			send_buffer_flag("flag_channel_B[1] = 1||35");
		}
		//-----------------------------------------------BC----------------------------------------------------
		if(((SIN_C_ref[kb0]*-1)-shift10) < vol[5] && ((SIN_C_ref[kb0]*-1)+shift10) > vol[5]){
			flag_channel_B[2] = 0;
			send_buffer_flag("flag_channel_B[2] = 0||36");
		}
		else{
			flag_channel_B[2] = 1;
			send_buffer_flag("flag_channel_B[2] = 1||37");
		}
	}

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
	InitSynchA();

	while(1){

				/*ADC_SoftwareStartConv(ADC1);
	            adc_value = ADC_GetConversionValue(ADC1);
	            sprintf(buffer, "%d\r\n", adc_value);
	            USARTSend(buffer, sizeof(buffer));*/

		channel_status();
		switch_channel();
		channel_A_ON();
		channel_B_ON();
		channel_A_OFF();
		channel_B_OFF();
		ButControl();
	}
}











