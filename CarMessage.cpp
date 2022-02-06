#include "MessageQueue.hpp"

// TODO: Consider making into a class properly
struct CarMessage : Message
{
    long state;
    MsgQueue *carQueue;
    bool result;
    std::string carName;

    CarMessage(long _l, std::string _s){
        this->state = _l;
        this->carQueue = nullptr;
        this->result = false;
        this->carName = _s;
    }

    CarMessage(long _l, bool _b, std::string _s){
        this->state = _l;
        this->carQueue = nullptr;
        this->result = _b;
        this->carName = _s;
    }

    CarMessage(long _l, MsgQueue* _MsQ, std::string _s){
        this->state = _l;
        this->carQueue = _MsQ;
        this->result = false;
        this->carName = _s;
    }

    CarMessage(long _l, MsgQueue* _MsQ, bool _b, std::string _s){
        this->state = _l;
        this->carQueue = _MsQ;
        this->result = _b;
        this->carName = _s;
    }
};