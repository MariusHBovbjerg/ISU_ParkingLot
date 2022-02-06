#pragma once
#include "Message.hpp"
#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <utility>
#include <string>

class MsgQueue
{
public:
    MsgQueue(unsigned long maxSize, std::string queueName);
    void send(Message *msg);
    Message *receive();
    ~MsgQueue();

private:
    std::queue<Message*> mq;
    unsigned long maxSize;
    std::string queueName;
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condRX = PTHREAD_COND_INITIALIZER;
    pthread_cond_t condTX = PTHREAD_COND_INITIALIZER;
};