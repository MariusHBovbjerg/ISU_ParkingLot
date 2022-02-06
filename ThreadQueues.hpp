#pragma once
#include "MessageQueue.hpp"

struct ThreadQueues
{
    MsgQueue *entryQueue;
    MsgQueue *exitQueue;
};