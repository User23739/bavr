/*
 * messages.h
 *
 *  Created on: 19 ����. 2018 �.
 *      Author: a.boyarkin
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#define MAX_MESSAGES 64

#define MSG_SOME_MESSAGES 0

void InitMessages(void);
void SendMessages(char Msg);
char GetMessages(char Msg);

#endif /* MESSAGES_H_ */
