#pragma once
#include "string"

enum ParkingLotStates{
    ST_entry_request_open,
    ST_car_in,
    ST_exit_request_open,
    ST_car_out,
    ST_entry_confirm_open,
    ST_exit_confirm_open
};

class Message
{
public:
	virtual ~Message() = default;
};