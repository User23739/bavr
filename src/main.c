
/*-----Инклуды--------- */

#include "main.h"

#define SHIFT_ZERO 5

//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[20] = {'\0'};  // буфер для передачи данных, примитивный
extern float buff_chanA1[201];
extern float buff_chanA2[201];
extern float buff_chanA3[201];

extern float buff_chanB1[201];
extern float buff_chanB2[201];
extern float buff_chanB3[201];
extern float aver_tmp_chan[7];
extern int a1;			//указатели буфера канала А
extern int b1;
extern float real_tmp_chan[7];





short flag_aktiv_channel = 0;				// 1 - канал А; 0 - оба выключенны; 2 - канал В.
short flag_status_chann_A = 0;				// состояние канала А;  0 - канал норм, 1 - канал не в норм.
short flag_status_chann_B = 0;				// состояние канала B;  0 - канал норм, 1 - канал не в норм.
short flag_switch_A = 0;		    		// 0 - вкл; 1 - откл
short flag_switch_B = 0;		    		// 0 - вкл; 1 - откл

short flag_mov_sin_A = 0;						    // 0 -положительная полуволна, 1 -отрицательная полуволна
short flag_mov_sin_B = 0;						    // 0 -положительная полуволна, 1 -отрицательная полуволна

short count_work_A_err = 0;  //счетчик ошибок синхронизации
short err_flag_sinch_A = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
short count_work_B_err = 0;  //счетчик ошибок синхронизации
short err_flag_sinch_B = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
short flag_sinch_chan_A = 0;					// 0-нет синхронизации; 1-есть синхронизация
short flag_sinch_chan_B = 0;					// 0-нет синхронизации; 1-есть синхронизация
short flag_zero_chan_A = 0;					// 0-"0"не найден; 1-"0" найден
short flag_zero_chan_B = 0;					// 0-"0"не найден; 1-"0" найден
short flag_compar_A = 0;					// "0"- не разрешается работа  "1"-положительная полуволна "2"- отрицательная полуволна
short flag_compar_B = 0;					//  "0"- не разрешается работа  "1"-положительная полуволна "2"- отрицательная полуволна


//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А
/*uint32_t SIN_A_ref_up[] = {1983,	2390, 2758,	3049, 3236,	3300, 3236,	3049, 2758,
					  2390,	1983, 1577,	1209, 918, 731,	666, 731, 918, 1209, 1577 };            //Полный период Резерв на всякий случай     */

float SIN_A_ref[] = {0, 13, 40, 66, 91, 116, 141, 164, 187, 208, 228, 247,
					 264, 279, 293, 305, 315, 323, 329, 333, 355, 335, 333,
						329, 323, 315, 305, 293, 279, 264, 247, 228, 208,
						187, 164, 141, 116, 91, 66, 40, 14};			// идеал положительной полуволны синусоиды

float SIN_B_ref[] = {-297,5,	-318,5, -331,5, -336, -333, -321, -301,5, -274,5, -241,
						-201,5, -157, -108,5, -57, -4,5, 49, 100, 149, 194, 234,5, 269,5};			// сдвиг по синусоиде В


float SIN_C_ref[] = {284,5, 253, 215, 172,5, 75, 22,5, -31, -83, -133, -179,5, -222, -258,5,
							-289, -312, -328, -335,5,  -335, -326, -309};						// сдвиг по синусоиде С



//переменные допуска по синусоиде 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 48;

float shift = 0;


short int flag_channel_A[3]={0};				//[0] - флаг состояния АА  0 - хорошо; 1 - плохая;
												//[1] - флаг состояния AB
												//[2] - флаг состояния AC

// предидущие значения
int kb = 0;                                  // счетчик измерений от ноля
int kb0 = 0;
int count_point = 0;						//счетчик отчетов

short int flag_channel_B[3]={0};				//[0] - флаг состояния BА   0 - хорошо; 1 - плохая;
												//[1] - флаг состояния BB
												//[2] - флаг состояния BC




//-------------------------
/*тестовый буфер флагов */
// буфер кольцево для хранения данных измерения
short buff_flag_[500] = {0};


int c_out = 0;

//указатель кольцевого буфера
int a11 = 0;



void send_buffer_flag(short *vol){

	if(a11 >= 501 ) a11 = 0;
	buff_flag_[a11] = vol;
	a11++;


}



void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {

    	GPIO_SetBits(LED1_PORT, LED1);   //бит установил
    	ProcessTimers();			//Вызываем функцию увеличения значений таймера
    	Control();						//Вызвываем функцию усреднения
    	GPIO_ResetBits(LED1_PORT, LED1);    //бит снял
    	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}


/*Функция контроллер*/
void Control(){

	TransInData();									//преобразование данных в удобный вид
	BuffData(&real_tmp_chan[0]);					// помещение данных в буфер
	//Aver();											// усреднение - фильтрация
	SinQuadrant(&real_tmp_chan[0]);	//положение синусоиды
	//ZeroDetect(&real_tmp_chan[0], shift20); 			//детектирование 0
	//SynchA();										//синхронизация
	//sin_compar_A(aver_tmp_chan, shift20);			//Вызываем функцию сравнения канала А
	//sin_compar_B(aver_tmp_chan);					//Вызываем функцию сравнения канала B



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



/*Функция синхронизации */
//Инициализация функции синхронизации
void InitSynchA(){
	count_work_A_err = 0;
	err_flag_sinch_A = 0;
	count_work_B_err = 0;
	err_flag_sinch_B = 0;

}
//Процесс функции инициализации
void SynchA (void){
	static int count_half_wave;		//счетчик полуволн
	static short zero_first_detect;	//первое детектирование "0"

	if (flag_zero_chan_A){
		if (!zero_first_detect){
				count_half_wave = 0;
				count_point = 0;
				zero_first_detect = 1;
				StartGTimer(GTIMER4);
			}
		send_buffer_flag(1);
		send_buffer_flag(GetGTimer(GTIMER4));
		}
	else{
		send_buffer_flag(2);
		send_buffer_flag(GetGTimer(GTIMER4));
		}





	/*if (flag_sinch_chan_A == 0){
		if ((REF_MIN < vol[0]) && (vol[0] < REF_MAX)){
			flag_sinch_chan_A = 1;
			send_buffer_flag(1);
			count_period = 0;
			err_flag_sinch_A = 0;
			count_work_A_err = 0;
			StartGTimer(GTIMER4);
		}
		else{
			count_work_A_err++;
			if(count_work_A_err < COUN_SINCH_ERR) err_flag_sinch_A = 1;
		}

	}

	/*if (flag_sinch_chan_B == 0){
			if ((REF_MIN < vol[3]) && (vol[3] < REF_MAX)){
				flag_sinch_chan_B = 1;
				send_buffer_flag(2);
				err_flag_sinch_B = 0;
				StartGTimer(GTIMER5);
			}
			else{
				count_work_B_err++;
				if(count_work_B_err < COUN_SINCH_ERR) err_flag_sinch_B = 1;
			}
		}*/

}

/*Функция определения квадранта синуса*/
void SinQuadrant(float *vol){
	//static int tmpA;
	//static int tmpB;
	//tmpA = x;
	//tmpB = y;
	if (SIN_A_ref[0] < vol[0]){
		flag_mov_sin_A = 0;
		send_buffer_flag(1);
	}
	else{
		flag_mov_sin_A = 1;
		send_buffer_flag(2);
	}
	/*if (buffB[tmpB] >= SIN_B_ref[0]){
		flag_mov_sin_B = 0;
	}
	else{
		flag_mov_sin_B = 1;
	}*/





	/*if((flag_sinch_chan_A == 1) && (count_period == 0)){
		if((tmpA == 0) ||(tmpA == 1)) {
			tmpA = 198;
		}
		else{
			tmpA--;
			tmpA--;
		}
		if(buffA[tmpA] > zero){
			flag_mov_sin_A = 0;
			//c_out = 1;
			send_buffer_flag(3);
		}
		else{
			flag_mov_sin_A = 1;
			//c_out = 1;
			send_buffer_flag(4);
		}
	}
	/*if(flag_sinch_chan_B == 1){
		tmpB--;
		if(buffB[tmpB] > zero){
			flag_mov_sin_B = 0;
			send_buffer_flag(5);
		}
		else{
			flag_mov_sin_B = 1;
			send_buffer_flag(6);
		}
	}*/


}
/*Функция детектирования 0*/
void ZeroDetect(float *vol, float shift){

	if(((SIN_A_ref[0]-shift) < vol[0]) && ((SIN_A_ref[0]+shift) > vol[0])){
		flag_zero_chan_A = 1;
		send_buffer_flag(1);
	}
	else{
		flag_zero_chan_A = 0;
		send_buffer_flag(2);
	}
	if(((SIN_B_ref[0]-shift) < vol[3]) && ((SIN_B_ref[0]+shift) > vol[3])){
		flag_zero_chan_B = 1;
	}
	else{
		flag_zero_chan_B = 0;
	}
}

/*Функция расчета частоты*/
void Freq(){

}


/*Функция расчета среднеквадратичного значения*/
void TrueRMS(){

}


//int k = 0;                                  // счетчик измерений от ноля
//int k0 = 0;									// переменная для хранения пред идущего указателя буфера




//---------- функция сравнения синуса канала A-----------------------------------------------------------
/// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.



/*void sin_compar_A(float  *vol, float *shift, int k){


	switch(flag_compar_A){
	case 1:
		if(((SIN_A_ref[k]-shift) < vol[0]) && ((SIN_A_ref[k]+shift) > vol[0])){
			flag_channel_A[0] = 0;
			send_buffer_flag(7);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)SIN_A_ref[k0]);
		}
		else{
			flag_channel_A[0] = 1;
			send_buffer_flag(9);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)(SIN_A_ref[k0]*-1));
		}
		break;
	case 2:
		if((((SIN_A_ref[k]*-1)-shift) < vol[0]) && (((SIN_A_ref[k]*-1)+shift) > vol[0])){
			flag_channel_A[0] = 0;
			send_buffer_flag(15);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)(SIN_A_ref[k0]*-1));
							}
		else{
			flag_channel_A[0] = 1;
			}
		break;
	default:
		break;
	}




/*	}*/
/*Сравнение синусоиды положительная полуволна*/
/*	if ((flag_mov_sin_A == 0) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if(((SIN_A_ref[k0]-shift) < vol[0]) && ((SIN_A_ref[k0]+shift) > vol[0])){
			flag_channel_A[0] = 0;
			send_buffer_flag(7);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)SIN_A_ref[k0]);
			count_period++;
			if (k0 >= 41){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				c_out = 0;
				send_buffer_flag(8);
				k0 = 0;
			}
		}
		else{
			flag_channel_A[0] = 1;
			send_buffer_flag(9);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)(SIN_A_ref[k0]*-1));
			k0++;
			if (k0 >= 41){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				c_out = 0;
				send_buffer_flag(10);
				k0 = 0;
			}
		}
		//---------------------------------------------AB------------------------------------------------------
		/*if((SIN_B_ref[k0]-shift10) < vol[1] && (SIN_B_ref[k0]+shift10) > vol[1]){
			flag_channel_A[1] = 0;
			send_buffer_flag(11);
		}
		else{
			flag_channel_A[1] = 1;
			send_buffer_flag(12);
		}
		//---------------------------------------------AC------------------------------------------------------
		if((SIN_C_ref[k0]-shift10) < vol[2] && (SIN_C_ref[k0]+shift10) > vol[2]) {
			flag_channel_A[2] = 0;
			send_buffer_flag(13);
		}
		else{
			flag_channel_A[2] = 1;
			send_buffer_flag(14);
		}*/
	/*}
/*Сравнение синусоиды отрицательная полуволна*/
/*	if ((flag_mov_sin_A == 1) && (flag_sinch_chan_A == 1)){
		//----------------------------------------------AA-----------------------------------------------------
		if((((SIN_A_ref[k0]*-1)-shift) < vol[0]) && (((SIN_A_ref[k0]*-1)+shift) > vol[0])){
			flag_channel_A[0] = 0;
			send_buffer_flag(15);
			//send_buffer_flag(k0);
			//send_buffer_flag((int)vol[0]);
			//send_buffer_flag((int)(SIN_A_ref[k0]*-1));
			k0++;
			if (k0 >= 41){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				c_out = 0;
				send_buffer_flag(16);
				k0 = 0;
			}
		}
		else{
			flag_channel_A[0] = 1;
			k0++;
			if (k0 >= 41){
				StopGTimer(GTIMER4);
				flag_sinch_chan_A = 0;
				c_out = 0;
				send_buffer_flag(17);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)(SIN_A_ref[k0]*-1));
				k0 = 0;
			}
		}
		//-----------------------------------------------AB----------------------------------------------------
		/*if(((SIN_B_ref[k0]*-1)-shift10) < vol[1] && ((SIN_A_ref[k0]*-1)+shift10) > vol[1]){
			flag_channel_A[1] = 0;
			send_buffer_flag(18);
		}
		else{
			flag_channel_A[1] = 1;
			send_buffer_flag(19);
		}
		//-----------------------------------------------AC----------------------------------------------------
		if(((SIN_C_ref[k0]*-1)-shift10) < vol[2] && ((SIN_C_ref[k0]*-1)+shift10) > vol[2]){
			flag_channel_A[2] = 0;
			send_buffer_flag(20);
		}
		else{
			flag_channel_A[2] = 1;
			send_buffer_flag(21);
		}*/
/*	}*/


/*}*/
//функция сравнения синусоиды канала B



//--------- функция сравнения синуса канала A------------------------------------------------------------
void sin_compar_B(float  *vol){
	if (kb0 >= 20){
		kb = 0;
		StopGTimer(GTIMER5);
		flag_sinch_chan_B = 0;
		send_buffer_flag(39);

	}
	/*if (kb == 0){
		kb = sizeof(SIN_A_ref)/sizeof(float);
		kb0 = 0;
	}
*/

/*Сравнение синусоиды положительная полуволна*/
	if ((flag_mov_sin_B == 0) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if((SIN_A_ref[kb0]-shift10) < vol[3] && (SIN_A_ref[kb0]+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			send_buffer_flag(22);
			kb0++;
			if (kb0 >= 20){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag(23);
			}
		}
		else{
			flag_channel_B[0] = 1;
			send_buffer_flag(24);
			kb0++;
			if (kb0 >= 20){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag(25);
			}
		}
		//---------------------------------------------BB------------------------------------------------------
		if((SIN_B_ref[kb0]-shift10) < vol[4] && (SIN_B_ref[kb0]+shift10) > vol[4]){
			flag_channel_B[1] = 0;
			send_buffer_flag(26);
		}
		else{
			flag_channel_B[1] = 1;
			send_buffer_flag(27);
		}
		//---------------------------------------------BC------------------------------------------------------
		if((SIN_C_ref[kb0]-shift10) < vol[5] && (SIN_C_ref[kb0]+shift10) > vol[5]) {
			flag_channel_B[2] = 0;
			send_buffer_flag(28);
		}
		else{
			flag_channel_B[2] = 1;
			send_buffer_flag(29);
		}
	}
/*Сравнение синусоиды отрицательная полуволна*/
	if ((flag_mov_sin_B == 1) && (flag_sinch_chan_B == 1)){
		//----------------------------------------------BA-----------------------------------------------------
		if(((SIN_A_ref[kb0]*-1)-shift10) < vol[3] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[3]){
			flag_channel_B[0] = 0;
			send_buffer_flag(30);
			kb0++;
			if (kb0 >= 20){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag(31);
			}
		}
		else{
			flag_channel_B[0] = 1;
			send_buffer_flag(32);
			kb0++;
			if (kb0 >= 20){
				StopGTimer(GTIMER5);
				flag_sinch_chan_B = 0;
				send_buffer_flag(33);
			}
		}
		//-----------------------------------------------BB----------------------------------------------------
		if(((SIN_B_ref[kb0]*-1)-shift10) < vol[4] && ((SIN_A_ref[kb0]*-1)+shift10) > vol[4]){
			flag_channel_B[1] = 0;
			send_buffer_flag(34);
		}
		else{
			flag_channel_B[1] = 1;
			send_buffer_flag(35);
		}
		//-----------------------------------------------BC----------------------------------------------------
		if(((SIN_C_ref[kb0]*-1)-shift10) < vol[5] && ((SIN_C_ref[kb0]*-1)+shift10) > vol[5]){
			flag_channel_B[2] = 0;
			send_buffer_flag(36);
		}
		else{
			flag_channel_B[2] = 1;
			send_buffer_flag(37);
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

		//channel_status();
		//switch_channel();
		//channel_A_ON();
		//channel_B_ON();
		//channel_A_OFF();
		//channel_B_OFF();
		//ButControl();
	}
}












