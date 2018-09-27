/*
 * process.h
 *
 *  Created on: 6 сент. 2018 г.
 *      Author: a.boyarkin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define REF_ZIRO 2048
#define U_QUANTUM 0.25513

void send_buffer(float *vol);
void channel_status(void);
void switch_channel(void);
void Aver(void);

#endif /* PROCESS_H_ */
