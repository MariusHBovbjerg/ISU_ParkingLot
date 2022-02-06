#include "MessageQueue.hpp"

enum ParkingLotStates{
    ST_entry_request_open,
    ST_car_in,
    ST_exit_request_open,
    ST_car_out,
    ST_entry_confirm_open,
    ST_exit_confirm_open
};

// TODO: Consider making into a class properly
struct CarMessage : Message
{
    long state;
    MsgQueue *carQueue = nullptr;
    bool result = false;
    std::string carName;

    CarMessage(long _l, std::string _s){
        this->state = _l;
        this->carName = _s;
    }

    CarMessage(long _l, bool _b, std::string _s){
        this->state = _l;
        this->result = _b;
        this->carName = _s;
    }

    CarMessage(long _l, MsgQueue* _MsQ, std::string _s){
        this->state = _l;
        this->carQueue = _MsQ;
        this->carName = _s;
    }

    CarMessage(long _l, MsgQueue* _MsQ, bool _b, std::string _s){
        this->state = _l;
        this->carQueue = _MsQ;
        this->result = _b;
        this->carName = _s;
    }
};