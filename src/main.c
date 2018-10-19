
/*-----�������--------- */

#include "main.h"

#define SHIFT_ZERO 12
#define CHANN 0

//-------����������� � ������� ��� ������------------------------------------
volatile char buffer[20] = {'\0'};  // ����� ��� �������� ������, �����������
extern float real_tmp_chan[7];





				// 1 - ����� �; 0 - ��� ����������; 2 - ����� �.


short flag_mov_sin[7] = {0};						    // 1 -������������� ���������, 0 -������������� ���������
int count_posit_point[7] = {0};
int count_nigativ_point[7] = {0};


short flag_sinch_ch = 0;  	 //���� ������������ �������
							 //0-�� ���������
							 //1-���������

short flag_zero[7] = {0};					// 0-"0"�� ������; 1-"0" ������
											//// [0]-����� � ���� 1
											// [1]-����� � ���� 2
											// [2]-����� � ���� 3
											// [3]-����� � ���� 1
											// [4]-����� � ���� 2
											// [5]-����� � ���� 3
											// [6]-����� � ���� 1
unsigned int rez_freg[7] = {0};				// ���������� ��� ������� �� ���� �������
float rezult[7] = {0};						//���������� ��� �������� �������� ����������.



//------������� ��������� � ��������� ����
//������������� �������������� �� ��������� �

float SIN_A_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228,
							247, 264, 279, 293, 305, 315, 323, 329, 333, 355,
							335, 333, 329, 323, 315, 305, 293, 279, 264, 247,
							228, 208, 187, 164, 141, 116, 91, 66, 40, 14};					// ����� ������������� ��������� ��������� �����������

float SIN_A_ref[] = {27, 53, 79, 104, 129, 153, 176, 198, 219, 238, 256,
					 272, 287, 300, 311, 320, 327, 332, 335, 336, 335, 332,
						327, 320, 311, 300, 287, 272, 256, 238, 219, 198,
						176, 153, 129, 104, 79, 53, 27, 1};									// ����� ������������� ��������� ���������



//���������� ������� �� ��������� 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 68;   //	������ 48

//float shift = 0;


//short int flag_channel_A[3]={0};
short int flag_channel[7] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

short int flag_channel_posit[7] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

short int flag_channel_negat[7] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

int  count_point[7] = {0};						//������� �������


//-------------------------
/*�������� ����� ������ */
// ����� �������� ��� �������� ������ ���������
short buff_flag_[1000] = {0};


//int c_out = 0;

//��������� ���������� ������
int a11 = 0;



void send_buffer_flag(short *vol){

	if(a11 >= 1001 ) a11 = 0;
	buff_flag_[a11] = vol;
	a11++;


}



void TIM4_IRQHandler(void){

    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {

    	GPIO_SetBits(LED1_PORT, LED1);   //��� ���������
    	ProcessTimers();			//�������� ������� ���������� �������� �������
    	Control();						//��������� ������� ����������
    	GPIO_ResetBits(LED1_PORT, LED1);    //��� ����
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





/*������� �������������� 0*/

void ZeroDetect(float *vol){



		//for (int i = 0; i<7; i++){

			/*if(((SIN_A_ref_aver[0]- SHIFT_ZERO ) < vol[i]) && ((SIN_A_ref_aver[0]+ SHIFT_ZERO ) > vol[i])){
			flag_zero[i] = 1;
			if (GetGTimer(GTIMER6 + i) >= MEG_POINT) StopGTimer(GTIMER6 + i);
			StartGTimer(GTIMER6  +i); //��������� ��������

			if(i == CHANN){
				GPIO_SetBits(LED2_PORT, LED2);   	//��� ���������
				//send_buffer_flag(1);
				GPIO_ResetBits(LED2_PORT, LED2);    //��� ����
			}

			}
			else{
				flag_zero[i] = 0;
*/

		static float after[7];
		static float before[7];

		for (int i = 0; i<7; i++){
			before[i] = after[i];
			after[i] = vol[i];

			if ((after[i] > 0) && (before[i] > 0)){
				flag_mov_sin[i] = 1;
			}
			else if ((after[i] < 0) && (before[i] > 0)){
				flag_mov_sin[i] = 0;
			}
			else if ((after[i] < 0) && (before[i] < 0)){
				flag_mov_sin[i] = 0;
			}
			else if ((after[i] > 0) && (before[i] < 0)){
				flag_mov_sin[i]= 1;
			}
/*���� ��� �����������
			if(i == CHANN){
					//GPIO_SetBits(LED2_PORT, LED2); //��� ���������
					//count_zero[i]++;
				if(flag_mov_sin[i]){
					send_buffer_flag(flag_mov_sin[i]);
				}
				else{
					send_buffer_flag(flag_mov_sin[i]);
				}


			}
*/
			if(flag_mov_sin[i]){
				count_nigativ_point[i] = 0;
				count_posit_point[i]++;
			}
			else{
				count_posit_point[i] = 0;
				count_nigativ_point[i]++;
			}

		}
		for (int i = 0; i<7; i++){
			if (count_nigativ_point[i] >100) count_nigativ_point[i] = 0;
			if (count_posit_point[i] >100) count_posit_point[i] = 0;
		}
}

/*������� ������� �������*/
 void Freq(){
	static unsigned int afterGT[7];
	static unsigned int beforeGT[7];
	//static unsigned int rez_freg[7];

	for (int i = 0; i<7; i++){
		if (flag_zero[i]){
			beforeGT[i] = afterGT[i];
			afterGT[i] = GetGTimer(GTIMER6 + i);
			rez_freg[i] =(1/(afterGT[i] - beforeGT[i])*250)/2;
		}
	}

}


/*������� ������� ������������������� ��������*/
void TrueRMS(float *vol){
	static float var_sum[7];
	//static float rezult[7] = {0};
	static int N[7];
	float var[7] = {0};

	for (int i=0; i<7; i++){
		if(!flag_zero[i]){
			N[i]++;
			var[i] = vol[i];
			var_sum[i] +=var[i]*var[i];
		}
		else{ rezult[i] =sqrt(var_sum[i]/N[i]);
			N[i] = 0;
		}
	}


}


//---------- ������� ��������� ������ ������ A-----------------------------------------------------------
/// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.





void SinCompar(float *vol, float shift){

	static int k[7];


	for (int i=0; i<7; i++){
		flag_channel_posit[i] = 0;
		flag_channel_negat[i] = 0;
	}


	for (int i=0; i<7; i++){

		/*k[i] = GetGTimer(GTIMER6 + i);

		if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
			flag_channel_posit[i] = 1;

			if(i == CHANN){
				send_buffer_flag(777);
			}

		}
		else{
			flag_channel_posit[i] = 0;
			if(i == CHANN){
				send_buffer_flag(999);
			}

		}

		if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
			flag_channel_negat[i] = 1;
			if(i == CHANN){
				send_buffer_flag(7777);
			}

		}
		else{
			flag_channel_negat[i] = 0;
			if(i == CHANN){
				send_buffer_flag(9999);
			}

			}

		if ((flag_channel_posit[i]) || (flag_channel_negat[i])){
			flag_channel[i] = 1;
		}
		else{
			flag_channel[i] = 0;
		}

*/









		switch(flag_mov_sin[i]){
		case 0:
			k[i] = count_nigativ_point[i];
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
				flag_channel_posit[i] = 1;

				if(i == CHANN){
					send_buffer_flag(777);
				}

			}
			else{
				flag_channel_posit[i] = 0;
				if(i == CHANN){
					send_buffer_flag(999);
				}

			}

			if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
				flag_channel_negat[i] = 1;
				if(i == CHANN){
					send_buffer_flag(7777);
				}

			}
			else{
				flag_channel_negat[i] = 0;
				if(i == CHANN){
					send_buffer_flag(9999);
				}

				}

			//����� �������� �������� �� ����

				if ((flag_channel_posit[i]) || (flag_channel_negat[i])){
					flag_channel[i] = 1;
				}
				else{
					flag_channel[i] = 0;
				}






			/*if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
				flag_channel[i] = 1;
				if(i == CHANN){
					send_buffer_flag(5);
					//send_buffer_flag(k0);
					send_buffer_flag((int)vol[i]);
					send_buffer_flag((int)SIN_A_ref[k[i]]);
			}
			}
			else{
				flag_channel[i] = 0;
				if(i == CHANN){ send_buffer_flag(9);
				//send_buffer_flag(k0);
				send_buffer_flag((int)vol[i]);
				send_buffer_flag((int)(SIN_A_ref[k[i]]*-1));
				}
			}*/
			break;
		case 1:
			k[i] = count_posit_point[i];
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
					flag_channel_posit[i] = 1;

					if(i == CHANN){
						send_buffer_flag(777);
					}

				}
				else{
					flag_channel_posit[i] = 0;
					if(i == CHANN){
						send_buffer_flag(999);
					}

				}

				if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (((SIN_A_ref[k[i]]*-1)+shift) > vol[i])){
					flag_channel_negat[i] = 1;
					if(i == CHANN){
						send_buffer_flag(7777);
					}

				}
				else{
					flag_channel_negat[i] = 0;
					if(i == CHANN){
						send_buffer_flag(9999);
					}

					}

				//����� �������� �������� �� ����

					if ((flag_channel_posit[i]) || (flag_channel_negat[i])){
						flag_channel[i] = 1;
					}
					else{
						flag_channel[i] = 0;
					}









			/*if (((SIN_A_ref[k[i]]-shift) < vol[i]) && ((SIN_A_ref[k[i]]+shift) > vol[i])){
				flag_channel[i] = 1;
				if(i == CHANN){ send_buffer_flag(15);
				//send_buffer_flag(k0);
				send_buffer_flag((int)vol[i]);
				send_buffer_flag((int)(SIN_A_ref[k[i]]*-1));
				}
			}
			else{
				flag_channel[i] = 0;
				if(i == CHANN){ send_buffer_flag(19);
				send_buffer_flag((int)vol[i]);
				send_buffer_flag((int)(SIN_A_ref[k[i]]*-1));
				}
				}*/
			break;
		default:
			break;
		}

	}
}


/*������� ����������*/
void Control(){
	static float tru_rms[7];
	static unsigned int freg[7];


	TransInData();									//�������������� ������ � ������� ���
	BuffData(&real_tmp_chan[0]);					// ��������� ������ � �����
	ZeroDetect(&real_tmp_chan[0]);
	Freq();
	TrueRMS(&real_tmp_chan[0]);
	SinCompar(&real_tmp_chan[0], shift20);			//�������� ������� ��������� ������ �
	ChannelStatus();								//����� ��������� �������
	SwitchChannel();								//���������� �������������� �������

	for (int i=0; i<7; i++){
		tru_rms[i] = rezult[i];
		freg[i] = rez_freg[i];
		}

}


int main(void){



// ������������� ���������

	SYSTEM_Init();
	RELAY_Init();
	ADC_DMA_Init();
	TIMER_Init();
	RS232_Init();
	InitGPIO();
	InitGTimers();  // �������������� ���������� �������
	InitKey();		//������������� ������� ������������ (����������)

	while(1){

		ButControl();
	}
}












