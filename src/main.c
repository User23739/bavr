
/*-----�������--------- */

#include "main.h"

#define SHIFT_ZERO 15

//-------����������� � ������� ��� ������------------------------------------
volatile char buffer[20] = {'\0'};  // ����� ��� �������� ������, �����������
//extern float buff_chanA1[201];
//extern float buff_chanA2[201];
//extern float buff_chanA3[201];

//extern float buff_chanB1[201];
//extern float buff_chanB2[201];
//extern float buff_chanB3[201];
//extern float aver_tmp_chan[7];
//extern int a1;			//��������� ������ ������ �
//extern int b1;
extern float real_tmp_chan[7];





				// 1 - ����� �; 0 - ��� ����������; 2 - ����� �.


short flag_mov_sin[7] = {0};						    // 0 -������������� ���������, 1 -������������� ���������
//short flag_mov_sin_B = 0;						    // 0 -������������� ���������, 1 -������������� ���������

//short count_work_A_err = 0;  //������� ������ �������������
//short err_flag_sinch_A = 0;	//o - ok; 1- ������ ���� ������ �������������
//short count_work_B_err = 0;  //������� ������ �������������
//short err_flag_sinch_B = 0;	//o - ok; 1- ������ ���� ������ �������������
//short flag_sinch_chan_A = 0;					// 0-��� �������������; 1-���� �������������
//short flag_sinch_chan_B = 0;					// 0-��� �������������; 1-���� �������������
short flag_zero[7] = {0};					// 0-"0"�� ������; 1-"0" ������
											//// [0]-����� � ���� 1
											// [1]-����� � ���� 2
											// [2]-����� � ���� 3
											// [3]-����� � ���� 1
											// [4]-����� � ���� 2
											// [5]-����� � ���� 3
											// [6]-����� � ���� 1
//short flag_zero_chan_B = 0;					// 0-"0"�� ������; 1-"0" ������
//short flag_compar_A = 0;					// "0"- �� ����������� ������  "1"-������������� ��������� "2"- ������������� ���������
//short flag_compar_B = 0;					//  "0"- �� ����������� ������  "1"-������������� ��������� "2"- ������������� ���������


//------������� ��������� � ��������� ����
//������������� �������������� �� ��������� �

float SIN_A_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228, 247,
					 264, 279, 293, 305, 315, 323, 329, 333, 355, 335, 333,
						329, 323, 315, 305, 293, 279, 264, 247, 228, 208,
						187, 164, 141, 116, 91, 66, 40, 14};					// ����� ������������� ��������� ��������� �����������

float SIN_A_ref[] = {27, 53, 79, 104, 129, 153, 176, 198, 219, 238, 256,
					 272, 287, 300, 311, 320, 327, 332, 335, 336, 335, 332,
						327, 320, 311, 300, 287, 272, 256, 238, 219, 198,
						176, 153, 129, 104, 79, 53, 27, 1};					// ����� ������������� ��������� ���������


/*float SIN_B_ref_aver[] = {-291, -297,5, -309, -318,5, -326, -331,5, -335,-336, -335,5, -333, -328,
		-321, -312, -301,5, -289, -274,5, -258,5, -241, -222, -201,5, -180, -157,
		-133, -108,5, -83, -57, -31, -4,5, 22,5, 49, 74,5, 100,
		125, 149, 172, 194, 215, 234,5, 253, 269,5};		// ����� �� ��������� �

float SIN_C_ref_aver[] = {13, 40, 66, 91, 116, 141, 164, 187, 208, 228, 247,
						 264, 279, 293, 305, 315, 323, 329, 333, 355, 335, 333,
							329, 323, 315, 305, 293, 279, 264, 247, 228, 208,
							187, 164, 141, 116, 91, 66, 40, 14};						// ����� �� ��������� �
*/


//���������� ������� �� ��������� 10%, 15%, 20%

float shift10 = 24;
float shift15 = 36;
float shift20 = 48;

//float shift = 0;


//short int flag_channel_A[3]={0};
short int flag_channel[7] = {0};		//[0] - ���� ��������� ��  0 - ������; 1 - ������;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ������; 1 - ������;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA


// ���������� ��������
//nt kb = 0;                                  // ������� ��������� �� ����
//int kb0 = 0;
int  count_point[7] = {0};						//������� �������

//short int flag_channel_B[3]={0};



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
		static float after[7];
		static float before[7];

		for (int i = 0; i<3; i++){
			before[i] = after[i];
			after[i] = vol[i];

			if(((SIN_A_ref_aver[0]- SHIFT_ZERO ) < vol[i]) && ((SIN_A_ref_aver[0]+ SHIFT_ZERO ) > vol[i])){
			flag_zero[i] = 1;
			count_point[i] = 0;
			GPIO_SetBits(LED2_PORT, LED2);   	//��� ���������
			send_buffer_flag(1);
			GPIO_ResetBits(LED2_PORT, LED2);    //��� ����
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

/*������� ������� �������*/
void Freq(){

}


/*������� ������� ������������������� ��������*/
void TrueRMS(){

}


//int k = 0;                                  // ������� ��������� �� ����
//int k0 = 0;									// ���������� ��� �������� ���� ������� ��������� ������




//---------- ������� ��������� ������ ������ A-----------------------------------------------------------
/// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.





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


/*������� ����������*/
void Control(){

	TransInData();									//�������������� ������ � ������� ���
	BuffData(&real_tmp_chan[0]);					// ��������� ������ � �����
	ZeroDetect(&real_tmp_chan[0]); 					//�������������� 0
	SinCompar(&real_tmp_chan[0], shift20);			//�������� ������� ��������� ������ �
	ChannelStatus();								//����� ��������� �������
	SwitchChannel();								//���������� �������������� �������


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

		channel_A_ON();
		//channel_B_ON();
		channel_A_OFF();
		//channel_B_OFF();
		//ButControl();
	}
}












