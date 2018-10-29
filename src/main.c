
/*-----Инклуды--------- */

#include "main.h"

#define SHIFT_ZERO 12
#define CHANN 0

//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[20] = {'\0'};  // буфер для передачи данных, примитивный
extern float real_tmp_chan[CHANN_W];





				// 1 - канал А; 0 - оба выключенны; 2 - канал В.


short flag_mov_sin[CHANN_W] = {0};						    // 1 -положительная полуволна, 0 -отрицательная полуволна
int count_posit_point[CHANN_W] = {0};
int count_nigativ_point[CHANN_W] = {0};


short flag_sinch_ch = 0;  	 //флаг синхронности каналов
							 //0-не синхронны
							 //1-синхронны

short flag_zero[CHANN_W] = {0};					// 0-"0"не найден; 1-"0" найден
											//// [0]-КАНАЛ А ФАЗА 1
											// [1]-КАНАЛ А ФАЗА 2
											// [2]-КАНАЛ А ФАЗА 3
											// [3]-КАНАЛ В ФАЗА 1
											// [4]-КАНАЛ В ФАЗА 2
											// [5]-КАНАЛ В ФАЗА 3
											// [6]-КАНАЛ С ФАЗА 1

short zero_noise[CHANN_W] = {0};					// 0 - нету дребезга; 1- дребезг;

unsigned int rez_freg[CHANN_W] = {0};				// переменная для частоты по всем каналам
float rezult_true_rms[CHANN_W] = {0};						//переменная для хранения значений напряжения.



//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А

float SIN_A_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228,
							247, 264, 279, 293, 305, 315, 323, 329, 333, 355,
							335, 333, 329, 323, 315, 305, 293, 279, 264, 247,
							228, 208, 187, 164, 141, 116, 91, 66, 40, 14};					// идеал положительной полуволны синусоиды усредненной

float SIN_A_ref[] = {27, 53, 79, 104, 129, 153, 176, 198, 219, 238, 256,
					 272, 287, 300, 311, 320, 327, 332, 335, 336, 335, 332,
						327, 320, 311, 300, 287, 272, 256, 238, 219, 198,
						176, 153, 129, 104, 79, 53, 27, 1};									// идеал положительной полуволны синусоиды



//переменные допуска по синусоиде 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 68;   //	должно 48

//float shift = 0;


//short int flag_channel_A[3]={0};
short int flag_channel[CHANN_W] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

short int flag_channel_posit[CHANN_W] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

short int flag_channel_negat[CHANN_W] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

int  count_point[CHANN_W] = {0};						//счетчик отчетов


//-------------------------
/*тестовый буфер флагов */
// буфер кольцево для хранения данных измерения
short buff_flag_[1000] = {0};


//int c_out = 0;

//указатель кольцевого буфера
int a11 = 0;


/*Функции для тестов */
void send_buffer_flag(short *vol){

	if(a11 >= 1001 ) a11 = 0;
	buff_flag_[a11] = vol;
	a11++;


}

/*генератор синусоиды*/



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

		static float after[CHANN_W];
		static float before[CHANN_W];
		int tmp_rez = 0;

		for (int i = 0; i<CHANN_W; i++){
			before[i] = after[i];
			after[i] = vol[i];

			if ((after[i] < ZERO_MAX) && (after[i] > ZERO_MIN)){
				flag_zero[i] += 1;
				if(flag_zero[i] == 2){
					zero_noise[i] = 1;
				}

			}
			else{
				flag_zero[i] = 0;
				zero_noise[i] = 0;
			}

			if ((after[i] > 0) && (before[i] > 0)){
				flag_mov_sin[i] = 1;
				if(i == CHANN){
					send_buffer_flag(777);
					tmp_rez = before[i] - after[i];
					send_buffer_flag(tmp_rez);
				}
				//count_posit_point[i]++;
				//count_nigativ_point[i] = 0;
			}
			else if ((after[i] < 0) && (before[i] > 0)){
				flag_mov_sin[i] = 0;
				if(i == CHANN){
					send_buffer_flag(6999);
					tmp_rez = before[i] - after[i];
					send_buffer_flag(tmp_rez);
				}
				//count_nigativ_point[i]++;
				//count_posit_point[i] = 0;
			}
			else if ((after[i] < 0) && (before[i] < 0)){
				flag_mov_sin[i] = 0;
				if(i == CHANN){
					send_buffer_flag(999);
					tmp_rez = after[i] - before[i];
					send_buffer_flag(tmp_rez);
				}
				//count_nigativ_point[i]++;
				//count_posit_point[i] = 0;
			}
			else if ((after[i] > 0) && (before[i] < 0)){
				flag_mov_sin[i]= 1;
				if(i == CHANN){
					send_buffer_flag(6777);
					tmp_rez = after[i] - before[i];
					send_buffer_flag(tmp_rez);
				}
				//count_posit_point[i]++;
				//count_nigativ_point[i] = 0;
			}
/*тест для определения
			if(i == CHANN){
					//GPIO_SetBits(LED2_PORT, LED2); //бит установил
					//count_zero[i]++;
				if(flag_mov_sin[i]){
					send_buffer_flag(flag_mov_sin[i]);
				}
				else{
					send_buffer_flag(flag_mov_sin[i]);
				}*/



			//рабочий метод только при синусоиде. при ноле  начинается дребезг
			if(flag_mov_sin[i]){
				count_nigativ_point[i] = 0;
				count_posit_point[i]++;
			}
			else{
				count_posit_point[i] = 0;
				count_nigativ_point[i]++;
			}
			if (count_nigativ_point[i] >41) count_nigativ_point[i] = 0;
			if (count_posit_point[i] >41) count_posit_point[i] = 0;

		}
		/*for (int i = 0; i<7; i++){
			if (count_nigativ_point[i] >40) count_nigativ_point[i] = 0;
			if (count_posit_point[i] >40) count_posit_point[i] = 0;
		}*/

}

/*Функция расчета частоты*/
void Freq(){

	for (int i = 0; i<CHANN_W; i++){
		if (flag_mov_sin[0]){
			StartGTimer(GTIMER3 + i);
			rez_freg[i] = (1/((float)GetGTimer(GTIMER9 + i)*0.00025))/2;
			StopGTimer(GTIMER9 + i);
		}
		else{
			StartGTimer(GTIMER9 + i);
			rez_freg[i] = (1/((float)GetGTimer(GTIMER3 + i)*0.00025))/2;
			StopGTimer(GTIMER3 + i);

		}

	}


}



/*Функция расчета среднеквадратичного значения*/
void TrueRMS(float *vol){
static float var_sum_p[CHANN_W];
static float var_sum_n[CHANN_W];
static int Np[CHANN_W];
static int Nn[CHANN_W];
float var[CHANN_W] = {0};

	for (int i=0; i<CHANN_W; i++){
		if(flag_mov_sin[i]){
			if(Nn[i]>=39){
				rezult_true_rms[i] =roundl(sqrt(var_sum_n[i]/Nn[i]));
				Nn[i] = 0;
				var_sum_n[i] = 0;
			}
			Np[i]++;
			var[i] = vol[i];
			var_sum_p[i] +=var[i]*var[i];
		}
		else{
			if(Np[i]>=39){
				rezult_true_rms[i] =roundl(sqrt(var_sum_p[i]/Np[i]));
				Np[i] = 0;
				var_sum_p[i] = 0;
			}
			Nn[i]++;
			var[i] = vol[i];
			var_sum_n[i] +=var[i]*var[i];

		}
	}

}

/*Функция определения синхронности каналов*/
void SinChanAB(float *vol){
float data_chan[CHANN_W] = {0};
for (int i=0; i<CHANN_W; i++){
	data_chan[i] = vol[i];
		}
	if (flag_mov_sin[0] == flag_mov_sin[3]){
		if (flag_mov_sin[1] == flag_mov_sin[4]){
			if (flag_mov_sin[2] == flag_mov_sin[5]){

			}
		}
	}
}
//---------- функция сравнения синуса канала A-----------------------------------------------------------
/// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.

void SinCompar(float *vol, float shift){

	static int k[CHANN_W];


	for (int i=0; i<CHANN_W; i++){
		flag_channel_posit[i] = 0;
		flag_channel_negat[i] = 0;
	}


	for (int i=0; i<CHANN_W; i++){
		switch(flag_mov_sin[i]){
		case 0:
			k[i] = count_nigativ_point[i];
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
				flag_channel_posit[i] = 1;
				/*if(i == CHANN){
					send_buffer_flag(k[i]);
				}*/
			}
			else{
				flag_channel_posit[i] = 0;
			}

			if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
				flag_channel_negat[i] = 1;
			}
			else{
				flag_channel_negat[i] = 0;
				}

			//нужно добавить преверку на ноль

				if (((flag_channel_posit[i]) || (flag_channel_negat[i])) && (!zero_noise[i])){
					flag_channel[i] = 1;
				}
				else{
					flag_channel[i] = 0;
				}
			break;
		case 1:
			k[i] = count_posit_point[i];
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
					flag_channel_posit[i] = 1;
				}
				else{
					flag_channel_posit[i] = 0;
				}

				if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
					flag_channel_negat[i] = 1;
				}
				else{
					flag_channel_negat[i] = 0;
				}

				//нужно добавить преверку на ноль

					if (((flag_channel_posit[i]) || (flag_channel_negat[i])) && (!zero_noise[i])){
						flag_channel[i] = 1;
					}
					else{
						flag_channel[i] = 0;
					}
			break;
		default:
			break;
		}

	}
}


/*Функция контроллер*/
void Control(){


	TransInData();									//преобразование данных в удобный вид// функция работает правильно
	BuffData(&real_tmp_chan[0]);					// помещение данных в буфер//функция работает бравильно
	ZeroDetect(&real_tmp_chan[0]);
	Freq();
	TrueRMS(&real_tmp_chan[0]);
	SinCompar(&real_tmp_chan[0], shift20);			//Вызываем функцию сравнения канала А
	ButControl();
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

		ButControl();
	}
}












