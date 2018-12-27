
/*-----�������--------- */

#include "main.h"
/*-----��������� ���������------*/

#define CHANN 5
#define ZERO_MAX 13
#define ZERO_MIN -13
#define COUN_SINCH_ERR 20
#define COUNT_END 10000
#define MEG_POINT 41

#define SHIFT_PHASE_ROT 20
#define FREG 50
#define SHIFT_FREG 2
#define SINCH_TRESHOLD 20
#define ZERO_MAX_SWITCH 20
#define ZERO_MIN_SWITCH -20
#define SWITCH_POINT 2
#define SWITCH_POINT_CRASH 8


//-------�����������------------------------------------
volatile char buffer[20] = {'\0'};  // ����� ��� �������� ������, �����������
extern float real_tmp_chan[CHANN_W];
//extern uint16_t ADCBuffer[7];
extern uint16_t  data_chan[CHANN_W];
extern unsigned short reg_data[30];
extern short status_chann_A;		    		// 0 - ����; 1 - ���
extern short status_chann_B;
extern short flag_priori_chann_manual;	//���������� ���������� ������
extern short flag_aktiv_channel;
extern float buff_chan[CHANN_W][BUFER_CIZE];
extern int a[CHANN_W];
extern float rezult_true_rms[CHANN_W];						//���������� ��� �������� �������� ����������.
extern short key;



				// 1 - ����� �; 0 - ��� ����������; 2 - ����� �.


short flag_mov_sin[CHANN_W] = {0};						    // 1 -������������� ���������, 0 -������������� ���������
int count_posit_point[CHANN_W] = {0};
int count_nigativ_point[CHANN_W] = {0};


short flag_sinch_ch = 0;  	 //���� ������������ �������
							 //0-�� ���������
							 //1-���������


short flag_phase_rot = 0;  	 //��� ������� ����������� ���
							 //0-������ �����������
							 //1-���������� �����������

short flag_zero[CHANN_W] = {0};				// 0-"0"�� ������; 1-"0" ������
											// [0]-����� � ���� 1
											// [1]-����� � ���� 2
											// [2]-����� � ���� 3
											// [3]-����� � ���� 1
											// [4]-����� � ���� 2
											// [5]-����� � ���� 3
											// [6]-����� � ���� 1

short flag_zero_freq[CHANN_W] = {0};		// 0-"0"�� ������; 1-"0" ������
											// [0]-����� � ���� 1
											// [1]-����� � ���� 2
											// [2]-����� � ���� 3
											// [3]-����� � ���� 1
											// [4]-����� � ���� 2
											// [5]-����� � ���� 3
											// [6]-����� � ���� 1

short flag_freq[2] = {0};					// 0-"0"������� �� � �����; 1-"1" ������� � �����
											// [0]-����� � ���� 1
											// [1]-����� � ���� 2



short zero_noise[CHANN_W] = {0};					// 0 - ���� ��������; 1- �������;

float rez_freg[CHANN_W] = {0};				// ���������� ��� ������� �� ���� �������

short flag_z_switch_crash = 0;		// ���������� ����������� ������������ ��� ����������� ��������� ���� ��� ������
short flag_z_switch = 0;		// ���������� ����������� ������������ ��� ����������� ���������� ���� ��� ������ ������




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

float shift10 = 34;
float shift15 = 51;
float shift20 = 68;
float shift = 51;                   //��������� ��������


short int flag_channel[CHANN_W] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

short int flag_channel_posit[CHANN_W] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

short int flag_channel_negat[CHANN_W] = {0};		//[0] - ���� ��������� ��  0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� AB
										//[2] - ���� ��������� AC
										//[0] - ���� ��������� B�   0 - ��������� �� � �����; 1 - ��������� � �����;
										//[1] - ���� ��������� BB
										//[2] - ���� ��������� BC
										//[0] - ���� ��������� CA

int  count_point[CHANN_W] = {0};						//������� �������


//-------------------------
/*�������� ����� ������ */

// ����� �������� ��� �������� ������ ���������
short buff_flag_[1000] = {0};

/*������� ��� ������ */

//���������� ��� �������� �������� ����������
float gen_data[CHANN_W] = {0}; // ���������� ���� ���������� ���������� ������
										// � ���
										// [0]-����� � ���� 1
										// [1]-����� � ���� 2
										// [2]-����� � ���� 3
										// [3]-����� � ���� 1
										// [4]-����� � ���� 2
										// [5]-����� � ���� 3
										// [6]-����� � ���� 1

unsigned int key_delay = 60;		// �������� ��������� ����
unsigned int ss_key_delay = 4;		// �������� ��������� �����������

//��������� ���������� ������
int a11 = 0;


void send_buffer_flag(short *vol){

	if(a11 >= 1001 ) a11 = 0;
	buff_flag_[a11] = vol;
	a11++;


}

/*��������� ���������*/
void GenSin(void){
	static float time_point[CHANN_W];	//��� �������� �������� �������
	float ampl_U = 336;    	//������������ ���������� �������� ����������
	float angul_vel = 314.1592654; 	//�������� �� ���������
	float period = 0.02; 			//�������� ������� �������� ����������
	float pi = 3.1415926535;

	angul_vel =(2*pi)/period;

	//��������� ������� �������, ��������� �������
	for (int i = 0; i<CHANN_W; i++){
		if(time_point[i] == 1.975) time_point[i] = 0;
	}

	//��������� 3-������ ���������
	for (int i = 0; i<CHANN_W; i++){
		if((i==0) || (i==3)) gen_data[i] = ampl_U*sin(angul_vel*time_point[i]); //���� �
		if((i==1) || (i==4)) gen_data[i] = ampl_U*sin((angul_vel*time_point[i])+4.1887); //���� �
		if((i==2) || (i==5)) gen_data[i] = ampl_U*sin((angul_vel*time_point[i])+2.094); //���� �
		time_point[i] += QUANT_POINT;
		}
}

void TransData(void){

	for (int i = 0; i<CHANN_W; i++){
		real_tmp_chan[i] = gen_data[i];
	}
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

		static float after[CHANN_W];
		static float before[CHANN_W];
		int tmp_rez = 0;

		for (int i = 0; i<CHANN_W; i++){
			before[i] = after[i];
			after[i] = vol[i];
			if ((after[i] > 0) && (before[i] > 0)){
				flag_zero_freq[i] = 1;
				flag_mov_sin[i] = 1;
				if(i == CHANN){
					send_buffer_flag(777);
					//tmp_rez = before[i] - after[i];
					//send_buffer_flag(tmp_rez);
				}
				//count_posit_point[i]++;
				//count_nigativ_point[i] = 0;
			}
			else if ((after[i] < 0) && (before[i] > 0)){
				flag_mov_sin[i] = 0;
				if(i == CHANN){
					send_buffer_flag(6999);
					//tmp_rez = before[i] - after[i];
					//send_buffer_flag(tmp_rez);
				}
				//count_nigativ_point[i]++;
				//count_posit_point[i] = 0;
			}
			else if ((after[i] < 0) && (before[i] < 0)){
				flag_zero_freq[i] = 0;
				flag_mov_sin[i] = 0;
				if(i == CHANN){
					send_buffer_flag(999);
					//tmp_rez = after[i] - before[i];
					//send_buffer_flag(tmp_rez);
				}
				//count_nigativ_point[i]++;
				//count_posit_point[i] = 0;
			}
			else if ((after[i] > 0) && (before[i] < 0)){
				flag_mov_sin[i]= 1;
				if(i == CHANN){
					send_buffer_flag(6777);
					//tmp_rez = after[i] - before[i];
					//send_buffer_flag(tmp_rez);
				}
				//count_posit_point[i]++;
				//count_nigativ_point[i] = 0;
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
				}*/
		//������� ����� ������ ��� ���������. ��� ����  ���������� �������
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

}
/*--------------������� ����������� �������� �� ������ ��������� ��� ����������� ���� */
void Noise(float *vol){
	for (int i = 0; i<CHANN_W; i++){
				if ((vol[i] < ZERO_MAX) && (vol[i] > ZERO_MIN)){
					flag_zero[i] += 1;
					if(flag_zero[i] == 2){
						zero_noise[i] = 1;
					}
				}
				else{
					flag_zero[i] = 0;
					zero_noise[i] = 0;
				}
		}

}
/*----------������� �������� �� ���������� ���������� �� ������� �������
 * ���� ��� ����������� ������������ ��� �� ���������� �������  ��� ������������*/
void FastSwitch(float *vol){
	static short flag_zero_switch;					// ���������� ����������� ������������ ��� ����������� ��������� ����
	static short flag_zero_switch_pl;				// �������������� ����������

	if ((vol[6] < ZERO_MAX_SWITCH) && (vol[6] > ZERO_MIN_SWITCH)){
		flag_zero_switch_pl += 1;
		if(flag_zero_switch_pl == SWITCH_POINT){
					flag_z_switch = 1;
		}
		if(flag_zero_switch_pl == SWITCH_POINT_CRASH){
			flag_z_switch_crash = 1;
		}
	}
	else{
		flag_zero_switch_pl = 0;
		flag_z_switch_crash = 0;
	}


}

/*������� ������� �������*/
void Freq(){
	static float sum_freg[CHANN_W];
	static float after[CHANN_W];
	static float befor[CHANN_W];
	for (int i = 0; i<CHANN_W; i++){
		if (flag_mov_sin[i]){
			StartGTimer(GTIMER3 + i);
			//after[i] = (1/((float)GetGTimer(GTIMER9 + i)*0.00025))/2;
			after[i] = GetGTimer(GTIMER10 + i);
			StopGTimer(GTIMER10 + i);
		}
		else{
			StartGTimer(GTIMER10 + i);
			//befor[i] = (1/((float)GetGTimer(GTIMER3 + i)*0.00025))/2;
			befor[i] = GetGTimer(GTIMER3 + i);
			StopGTimer(GTIMER3 + i);
		}
		rez_freg[i] = 4000/(after[i]+befor[i]);
	}
}

void FreqCompar() {
	short int freq[CHANN_W] = {0};

	for (int i = 0; i<CHANN_W; i++){
		if((rez_freg[i] <= FREG + SHIFT_FREG) && (rez_freg[i] >= FREG - SHIFT_FREG)){
			freq[i] = 1;
		}
		else{
			freq[i] = 0;
		}

	}

	if((freq[0]) && (freq[1]) && (freq[2])){
		flag_freq[0] = 1;
	}
	else{
		flag_freq[0] = 0;
	}

	if((freq[3]) && (freq[4]) && (freq[5])){
			flag_freq[1] = 1;
		}
		else{
			flag_freq[1] = 0;
		}

	/*bool freq[CHANN_W];
	for (int i = 0; i<CHANN_W; i++){
		freq[i] = approximatelyEqual(rez_freg[i], FREG, 0.04);
	}*/
}




/*������� ����������� ������������ �������*/
void SinChanAB(){
		if(count_nigativ_point[0] == count_nigativ_point[3]){
			if(count_nigativ_point[1] == count_nigativ_point[4]){
				/*if(count_nigativ_point[2] == count_nigativ_point[5]){*/
					flag_sinch_ch = 1;
				/*}*/
			}
		}
		else{
			flag_sinch_ch = 0;
		}


}

// ���������� true, ���� ������� ����� a � b � �������� �������� ��������
/*bool approximatelyEqual(float a, float b, float epsilon)
{
    return fabsf(a - b) <= ( (fabsf(a) < fabsf(b) ? fabsf(b) : fabsf(a)) * epsilon);
}
*/

/*������� ����������� ����������� ��� */
void PhaseRot(float *vol){
	float data_chan[CHANN_W] = {0};
	short flag_phase[3] = {0};
	for (int i=0; i<CHANN_W; i++){
		data_chan[i] = vol[i];
			}
	if (((data_chan[0] + SHIFT_PHASE_ROT) >= data_chan[3]) && ((data_chan[0] - SHIFT_PHASE_ROT) <= data_chan[3])){
		flag_phase[0] = 1;
	}
	else{
		flag_phase[0] = 0;
	}
	if (((data_chan[1] + SHIFT_PHASE_ROT) >= data_chan[4]) && ((data_chan[1] - SHIFT_PHASE_ROT) <= data_chan[4])){
		flag_phase[0] = 1;
	}
	else{
		flag_phase[0] = 0;
	}
	if (((data_chan[2] + SHIFT_PHASE_ROT) >= data_chan[5]) && ((data_chan[2] - SHIFT_PHASE_ROT) <= data_chan[5])){
		flag_phase[0] = 1;
	}
	else{
		flag_phase[0] = 0;
	}



	flag_phase_rot;
}
void MbRead(void){
static unsigned int after_reg[4];
static unsigned int befor_reg[4];
static short int flag_stat_reg[4];

	for (int i=0; i<4; i++){
		befor_reg[i] = after_reg[i];
		after_reg[i] = reg_data[40 + i];

	}
	for (int i=0; i<4; i++){
		if (befor_reg[i] == after_reg[i]){
			flag_stat_reg[i] = 1 ;
		}
		else{
			flag_stat_reg[i] = 0 ;
					}
		}




	if(!flag_stat_reg[0]){
		switch(reg_data[40]){
				case KEY_STAT_SW_RELAY:
					key = KEY_STAT_SW_RELAY;
					break;
				case KEY_STAT_SW:
					key = KEY_STAT_SW;
					break;
				case KEY_RELAY:
					key = KEY_RELAY;
					break;
				default:
					break;
			}
	}
	if(!flag_stat_reg[1]){
		switch(reg_data[41]){
			case 0:
				shift = shift10;
				break;
			case 1:
				shift = shift15;
				break;
			case 2:
				shift = shift20;
				break;
			default:
				break;
		}
	}
	if(!flag_stat_reg[2]){
		if((reg_data[42] >= 0) && (reg_data[42] <= 4000000)){

				key_delay = (reg_data[42]*4)-1;

		}
	}
	if(!flag_stat_reg[3]){
		if((reg_data[43] > 0) && (reg_data[43] < 4000000)){

			ss_key_delay = (reg_data[43]*4)-1;

		}
	}

}

void MbWrite(void){
	reg_data[0] = flag_sinch_ch;
	reg_data[1] = status_chann_A;
	reg_data[2] = status_chann_B;
	reg_data[3] = flag_priori_chann_manual;
	reg_data[4] = flag_aktiv_channel;
	reg_data[5] = key;
	reg_data[6] = shift;



	for (int i = 0; i<CHANN_W; i++){
		reg_data[i+10] = rezult_true_rms[i];
		}
	for (int i = 0; i<CHANN_W; i++){
		reg_data[i+20] = rez_freg[i];
		}
	reg_data[30] = (key_delay/4)+1;
	reg_data[31] = (ss_key_delay/4)+1;
}

//---------- ������� ��������� ������ ������ A-----------------------------------------------------------
/// �������� ��������� ���� 7 �������. ������������� ����� �� 1 ����.

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
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && (vol[i] < (SIN_A_ref[k[i]]+shift))){
				flag_channel_posit[i] = 1;
			}
			else{
				flag_channel_posit[i] = 0;
			}
			if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && (vol[i] < ((SIN_A_ref[k[i]]*-1)+shift))){
				flag_channel_negat[i] = 1;
			}
			else{
				flag_channel_negat[i] = 0;
			}
				if (((flag_channel_posit[i]) || (flag_channel_negat[i])) && (!zero_noise[i])){
					flag_channel[i] = 1;
					if(i == CHANN){
						send_buffer_flag(1);
					}
				}
				else{
					flag_channel[i] = 0;
					if(i == CHANN){
						send_buffer_flag(2);
					}
				}
			break;
		case 1:
			k[i] = count_posit_point[i];
			if (((SIN_A_ref[k[i]]-shift) < vol[i]) && (vol[i] < (SIN_A_ref[k[i]]+shift))){
					flag_channel_posit[i] = 1;
				}
				else{
					flag_channel_posit[i] = 0;
				}

				if ((((SIN_A_ref[k[i]]*-1)-shift) < vol[i]) && ((vol[i] < (SIN_A_ref[k[i]]*-1)+shift))){
					flag_channel_negat[i] = 1;
				}
				else{
					flag_channel_negat[i] = 0;
				}

				//����� �������� �������� �� ����

					if (((flag_channel_posit[i]) || (flag_channel_negat[i])) && (!zero_noise[i])){
						flag_channel[i] = 1;
						if(i == CHANN){
							send_buffer_flag(111);
						}
					}
					else{
						flag_channel[i] = 0;
						if(i == CHANN){
							send_buffer_flag(222);
						}
					}
			break;
		default:
			break;
		}

	}
}


/*������� ����������*/
void Control(){
	//shift = shift10;
	//TransRawDataToBuffer(&data_chan[0]);			//�������� ������� ��� �������� ������ �� ���
	TransInData();									//�������������� ������ � ������� ���// ������� �������� ���������
	//------------------������� ��� �����, ����� �� ���������� ���������� ��������������� �������: ADC_DMA_Init();// TransInData();
	//GenSin();
	//TransData();
	//----------------------------------------
	BuffData(&real_tmp_chan[0]);					// ��������� ������ � �����//������� �������� ���������
	ZeroDetect(&real_tmp_chan[0]);
	Noise(&real_tmp_chan[0]);
	Freq();
	FreqCompar();
	//SinChanAB();
	SinCompar(&real_tmp_chan[0], shift);			//��������� ��������
	ChannelStatus();								//����� ��������� �������
	//FastSwitch(&real_tmp_chan[0]);
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
		GPIO_SetBits(LED2_PORT, LED2);
		MbRead();

		ButControl();
		TrueRMS();
		MbWrite();
		do_modbus();
		PulsOffPolRelay();
		PulsOffStatSwitch();
		GPIO_ResetBits(LED2_PORT, LED2);

	}
}












