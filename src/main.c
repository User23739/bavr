
/*-----Инклуды--------- */

#include "main.h"

#define SHIFT_ZERO 15

//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[20] = {'\0'};  // буфер для передачи данных, примитивный
//extern float buff_chanA1[201];
//extern float buff_chanA2[201];
//extern float buff_chanA3[201];

//extern float buff_chanB1[201];
//extern float buff_chanB2[201];
//extern float buff_chanB3[201];
//extern float aver_tmp_chan[7];
//extern int a1;			//указатели буфера канала А
//extern int b1;
extern float real_tmp_chan[7];





				// 1 - канал А; 0 - оба выключенны; 2 - канал В.


short flag_mov_sin[7] = {0};						    // 0 -положительная полуволна, 1 -отрицательная полуволна
//short flag_mov_sin_B = 0;						    // 0 -положительная полуволна, 1 -отрицательная полуволна

//short count_work_A_err = 0;  //счетчик ошибок синхронизации
//short err_flag_sinch_A = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
//short count_work_B_err = 0;  //счетчик ошибок синхронизации
//short err_flag_sinch_B = 0;	//o - ok; 1- Ошибка флаг ошибки синхронизации
//short flag_sinch_chan_A = 0;					// 0-нет синхронизации; 1-есть синхронизация
//short flag_sinch_chan_B = 0;					// 0-нет синхронизации; 1-есть синхронизация
short flag_zero[7] = {0};					// 0-"0"не найден; 1-"0" найден
											//// [0]-КАНАЛ А ФАЗА 1
											// [1]-КАНАЛ А ФАЗА 2
											// [2]-КАНАЛ А ФАЗА 3
											// [3]-КАНАЛ В ФАЗА 1
											// [4]-КАНАЛ В ФАЗА 2
											// [5]-КАНАЛ В ФАЗА 3
											// [6]-КАНАЛ С ФАЗА 1
//short flag_zero_chan_B = 0;					// 0-"0"не найден; 1-"0" найден
//short flag_compar_A = 0;					// "0"- не разрешается работа  "1"-положительная полуволна "2"- отрицательная полуволна
//short flag_compar_B = 0;					//  "0"- не разрешается работа  "1"-положительная полуволна "2"- отрицательная полуволна


//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А

float SIN_A_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228, 247,
					 264, 279, 293, 305, 315, 323, 329, 333, 355, 335, 333,
						329, 323, 315, 305, 293, 279, 264, 247, 228, 208,
						187, 164, 141, 116, 91, 66, 40, 14};					// идеал положительной полуволны синусоиды усредненной

float SIN_A_ref[] = {27, 53, 79, 104, 129, 153, 176, 198, 219, 238, 256,
					 272, 287, 300, 311, 320, 327, 332, 335, 336, 335, 332,
						327, 320, 311, 300, 287, 272, 256, 238, 219, 198,
						176, 153, 129, 104, 79, 53, 27, 1};					// идеал положительной полуволны синусоиды


/*float SIN_B_ref_aver[] = {-291, -297,5, -309, -318,5, -326, -331,5, -335,-336, -335,5, -333, -328,
		-321, -312, -301,5, -289, -274,5, -258,5, -241, -222, -201,5, -180, -157,
		-133, -108,5, -83, -57, -31, -4,5, 22,5, 49, 74,5, 100,
		125, 149, 172, 194, 215, 234,5, 253, 269,5};		// сдвиг по синусоиде В

float SIN_C_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228, 247,
						 264, 279, 293, 305, 315, 323, 329, 333, 355, 335, 333,
							329, 323, 315, 305, 293, 279, 264, 247, 228, 208,
							187, 164, 141, 116, 91, 66, 40, 14};						// сдвиг по синусоиде С
*/


//переменные допуска по синусоиде 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 48;

//float shift = 0;


//short int flag_channel_A[3]={0};
short int flag_channel[7] = {0};		//[0] - флаг состояния АА  0 - хорошо; 1 - плохая;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - хорошо; 1 - плохая;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA


// предидущие значения
//nt kb = 0;                                  // счетчик измерений от ноля
//int kb0 = 0;
int  count_point[7] = {0};						//счетчик отчетов

//short int flag_channel_B[3]={0};



//-------------------------
/*тестовый буфер флагов */
// буфер кольцево для хранения данных измерения
short buff_flag_[1000] = {0};


//int c_out = 0;

//указатель кольцевого буфера
int a11 = 0;



void send_buffer_flag(short *vol){

	if(a11 >= 1001 ) a11 = 0;
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




void USARTSend(const unsigned char *pucBuffer){
	while (*pucBuffer)
	{
		USART_SendData(USART1, *pucBuffer++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
		}
	}
}





/*Функция детектирования 0*/

void ZeroDetect(float *vol){
		static float after[7];
		static float before[7];

		for (int i = 0; i<3; i++){
			before[i] = after[i];
			after[i] = vol[i];

			if(((SIN_A_ref_aver[0]- SHIFT_ZERO ) < vol[i]) && ((SIN_A_ref_aver[0]+ SHIFT_ZERO ) > vol[i])){
			flag_zero[i] = 1;
			count_point[i] = 0;
			GPIO_SetBits(LED2_PORT, LED2);   	//бит установил
			send_buffer_flag(1);
			GPIO_ResetBits(LED2_PORT, LED2);    //бит снял
			if ((after[i] > 0) && (before[i] > 0)){
				flag_mov_sin[i] = 1;
			}
			else if ((after[i] < 0) && (before[i] > 0)){
				flag_mov_sin[i] = 1;
			}
			else if ((after[i] < 0) && (before[i] < 0)){
				flag_mov_sin[i] = 0;
			}
			else if ((after[i] > 0) && (before[i] < 0)){
				flag_mov_sin[i]= 0;
			}
			}
			else{
				flag_zero[i] = 0;
				count_point[i]++;
				send_buffer_flag(2);
			}
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





void SinCompar(float *vol, float shift){

	static int k[7];

	for (int i=0; i<7; i++){

		k[i] = count_point[i];
		switch(flag_mov_sin[i]){
		case 0:
			if (((SIN_A_ref_aver[k[i]]-shift) < vol[i]) && ((SIN_A_ref_aver[k[i]]+shift) > vol[i])){
				flag_channel[i] = 0;
				send_buffer_flag(7);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)SIN_A_ref_aver[k0]);
			}
			else{
				flag_channel[i] = 1;
				send_buffer_flag(9);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)(SIN_A_ref_aver[k0]*-1));
			}
			break;
		case 1:
			if ((((SIN_A_ref_aver[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref_aver[k[i]]*-1)+shift) > vol[i])){
				flag_channel[i] = 0;
				send_buffer_flag(15);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)(SIN_A_ref_aver[k0]*-1));
								}
			else{
				flag_channel[i] = 1;
				send_buffer_flag(17);
				}
			break;
		default:
			break;
		}
	}



}


/*Функция контроллер*/
void Control(){

	TransInData();									//преобразование данных в удобный вид
	BuffData(&real_tmp_chan[0]);					// помещение данных в буфер
	ZeroDetect(&real_tmp_chan[0]); 					//детектирование 0
	SinCompar(&real_tmp_chan[0], shift20);			//Вызываем функцию сравнения канала А
	ChannelStatus();								//Опрос состояния каналов
	SwitchChannel();								//Управление переключениями каналов


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

		channel_A_ON();
		//channel_B_ON();
		channel_A_OFF();
		//channel_B_OFF();
		//ButControl();
	}
}












