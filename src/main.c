
/*-----Инклуды--------- */

#include "main.h"

#define SHIFT_ZERO 15
#define CHANN 2

//-------переменныеи и функции для тестов------------------------------------
volatile char buffer[20] = {'\0'};  // буфер для передачи данных, примитивный
extern float real_tmp_chan[7];





				// 1 - канал А; 0 - оба выключенны; 2 - канал В.


short flag_mov_sin[7] = {0};						    // 0 -положительная полуволна, 1 -отрицательная полуволна


short flag_sinch_ch = 0;  	 //флаг синхронности каналов
							 //0-не синхронны
							 //1-синхронны

short flag_zero[7] = {0};					// 0-"0"не найден; 1-"0" найден
											//// [0]-КАНАЛ А ФАЗА 1
											// [1]-КАНАЛ А ФАЗА 2
											// [2]-КАНАЛ А ФАЗА 3
											// [3]-КАНАЛ В ФАЗА 1
											// [4]-КАНАЛ В ФАЗА 2
											// [5]-КАНАЛ В ФАЗА 3
											// [6]-КАНАЛ С ФАЗА 1


//------Опорная синусоида в табличном виде
//Синхронизация осуществляется по синусоиде А

float SIN_A_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228,
							247, 264, 279, 293, 305, 315, 323, 329, 333, 355,
							335, 333, 329, 323, 315, 305, 293, 279, 264, 247,
							228, 208, 187, 164, 141, 116, 91, 66, 40, 14};					// идеал положительной полуволны синусоиды усредненной

float SIN_A_ref[] = {27, 53, 79, 104, 129, 153, 176, 198, 219, 238, 256,
					 272, 287, 300, 311, 320, 327, 332, 335, 336, 335, 332,
						327, 320, 311, 300, 287, 272, 256, 238, 219, 198,
						176, 153, 129, 104, 79, 53, 27, 1};					// идеал положительной полуволны синусоиды



//переменные допуска по синусоиде 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 48;

//float shift = 0;


//short int flag_channel_A[3]={0};
short int flag_channel[7] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

short int flag_channel_posit[7] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

short int flag_channel_negat[7] = {0};		//[0] - флаг состояния АА  0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния AB
										//[2] - флаг состояния AC
										//[0] - флаг состояния BА   0 - синусоида не в норме; 1 - синусоида в норме;
										//[1] - флаг состояния BB
										//[2] - флаг состояния BC
										//[0] - флаг состояния CA

int  count_point[7] = {0};						//счетчик отчетов


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
		//static float after[7];
		//static float before[7];

		for (int i = 0; i<7; i++){
			//before[i] = after[i];
			//after[i] = vol[i];
			if (count_point[i]>=41) count_point[i]=0;

			if(((SIN_A_ref_aver[0]- SHIFT_ZERO ) < vol[i]) && ((SIN_A_ref_aver[0]+ SHIFT_ZERO ) > vol[i])){
			flag_zero[i] = 1;
			count_point[i] = 0;
			if(i == CHANN){
				GPIO_SetBits(LED2_PORT, LED2);   	//бит установил
				//send_buffer_flag(1);
				GPIO_ResetBits(LED2_PORT, LED2);    //бит снял
			}
			/*if ((after[i] > 0) && (before[i] > 0)){
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
			}*/
			}
			else{
				flag_zero[i] = 0;
				count_point[i]++;
				//if(i == CHANN) send_buffer_flag(2);

			}
		}
}

/*Функция расчета частоты*/
void Freq(){

}


/*Функция расчета среднеквадратичного значения*/
/*float TrueRMS(float *vol){
	static float var_sum[7] = {0};
	static float rezult[7] = {0};
	static int N = 0;
	float var[7] = {0};
	N++;
	if(N < 40){
		for (int i=0; i<7; i++) var_sum[i] +=var[i]*var[i];
	}
	else{ for (int i = 0; i<7; i++) rezult[i] =sqrt(var_sum[i]/N) ;
	}

	return &rezult[0];
}
*/

//---------- функция сравнения синуса канала A-----------------------------------------------------------
/// передаем заначения всех 7 каналов. Синхронизацию ведем по 1 фазе.





void SinCompar(float *vol, float shift){

	static int k[7];
	int k_err[7] = {0};
	for (int i=0; i<7; i++){
		flag_channel_posit[i] = 0;
		flag_channel_negat[i] = 0;
	}

	for (int i=0; i<7; i++){

		k[i] = count_point[i];

		if (((SIN_A_ref_aver[k[i]]-shift) < vol[i]) && ((SIN_A_ref_aver[k[i]]+shift) > vol[i])){
			flag_channel_posit[i] = 1;

		}
		else{
			flag_channel_posit[i] = 0;

		}

		if ((((SIN_A_ref_aver[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref_aver[k[i]]*-1)+shift) > vol[i])){
			flag_channel_negat[i] = 1;

		}
		else{
			flag_channel_negat[i] = 0;

			}


		if ((flag_channel_posit[i]) || (flag_channel_negat[i])){
			flag_channel[i] = 1;
		}
		else{
			flag_channel[i] = 0;
		}












		/*switch(flag_mov_sin[i]){
		case 0:
			if (((SIN_A_ref_aver[k[i]]-shift) < vol[i]) && ((SIN_A_ref_aver[k[i]]+shift) > vol[i])){
				flag_channel[i] = 1;
				//if(i == CHANN) send_buffer_flag(7);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)SIN_A_ref_aver[k0]);
			}
			else{
				flag_channel[i] = 0;
				//if(i == CHANN) send_buffer_flag(9);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)(SIN_A_ref_aver[k0]*-1));
			}
			break;
		case 1:
			if ((((SIN_A_ref_aver[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref_aver[k[i]]*-1)+shift) > vol[i])){
				flag_channel[i] = 1;
				//if(i == CHANN) send_buffer_flag(15);
				//send_buffer_flag(k0);
				//send_buffer_flag((int)vol[0]);
				//send_buffer_flag((int)(SIN_A_ref_aver[k0]*-1));
								}
			else{
				flag_channel[i] = 0;
				//if(i == CHANN) send_buffer_flag(17);
				}
			break;
		default:
			break;
		}*/
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

		ButControl();
	}
}












