#pragma once
#include "CarMessage.cpp"
#include "ThreadQueues.hpp"

void entryMsgHandler(CarMessage *);
void exitMsgHandler(CarMessage *);
void carMsgHandler(CarMessage *, ThreadQueues *, MsgQueue *);