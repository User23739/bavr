/*
 * process.h
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef PROCESS_H_
#define PROCESS_H_




void TransInData(void);
void BuffData(float *vol);
void ChannelStatus(void);
void SwitchChannel(void);
void InitStatSwRelaySwitch();
void StatSwRelaySwitch(void);
void StatSwSwitch(void);
void RelaySwitch(void);
void StatSwRelayPolSwitch(void);
void TransRawDataToBuffer(uint16_t *vol);
void TrueRMS();
//void Aver(void);

#endif /* PROCESS_H_ */
