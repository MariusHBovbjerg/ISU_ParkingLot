#include "MessageQueue.hpp"

MsgQueue::MsgQueue(unsigned long maxSize, std::string queuename) : maxSize(maxSize), queueName(queuename) {}
MsgQueue::~MsgQueue() {}

Message *MsgQueue::receive()
{
    pthread_mutex_lock(&mut);

    while (mq.empty())
    {
        pthread_cond_wait(&condTX, &mut);
    }

    Message *val = mq.front();

    mq.pop();

    pthread_cond_signal(&condRX);

    pthread_mutex_unlock(&mut);

    return val;
}

void MsgQueue::send(Message *msg)
{
    pthread_mutex_lock(&mut);

    while (mq.size() >= maxSize)
    {
        pthread_cond_wait(&condRX, &mut);
    }

    mq.push(msg);

    pthread_cond_signal(&condTX);
    pthread_mutex_unlock(&mut);
}