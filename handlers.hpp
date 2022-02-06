#pragma once
#include "CarMessage.cpp"

void entryMsgHandler(CarMessage *);
void exitMsgHandler(CarMessage *);
void carMsgHandler(CarMessage *, MsgQueue *, MsgQueue *, MsgQueue *);