#include "handlers.hpp"

void entryMsgHandler(CarMessage *msg)
{
	switch (msg->state)
	{
		//ENTRY_OPEN_REQUEST
		case ST_entry_request_open:
		{
			CarMessage *entryConfirm = new CarMessage(ST_entry_confirm_open, true, msg->carName);

			printf("Entry: Door Opens for car %s.\n", msg->carName.c_str());

			msg->carQueue->send(entryConfirm);

			break;
		}

		//CAR_IN
		case ST_car_in:
		{
			printf("Entry: Door Closes for car %s.\n", msg->carName.c_str());
			break;
		}

		default:
			break;
	}
	delete msg;
}

void exitMsgHandler(CarMessage *msg)
{
	switch (msg->state)
	{
		//EXIT_OPEN_REQUEST
		case ST_exit_request_open:
		{
			CarMessage *exitConfirm = new CarMessage(ST_exit_confirm_open, true, msg->carName);

			printf("Exit: Door Opens for car %s.\n", msg->carName.c_str());

			msg->carQueue->send(exitConfirm);

			break;
		}
		//CAR_OUT
		case ST_car_out:
		{
			printf("Exit: Door Closes for car %s.\n", msg->carName.c_str());
			break;
		}
	
		default:
			break;
	}
	delete msg;
}

void carMsgHandler(CarMessage *msg, MsgQueue *entry, MsgQueue *exit, MsgQueue *carPTR)
{
	if (msg->result)
	{
		switch (msg->state)
		{
		// ENTRY_OPEN_CONFIRM
			case ST_entry_confirm_open:
			{
				
				//Car can drive in
				printf("Car %s: Drives into the parking lot.\n", msg->carName.c_str());
					
				CarMessage *carInsideLot = new CarMessage(ST_car_in, msg->carName);

				//Car is inside
				entry->send(carInsideLot);

				//Car sleeps in the parking lot
				sleep(rand() % 1 + 3);

				//exit request
				CarMessage *exitReq = new CarMessage(ST_exit_request_open, carPTR, msg->carName);

				printf("Car %s: Requests Exit open Door.\n", msg->carName.c_str());
				exit->send(exitReq);
				
				break;
			}
			//EXIT_OPEN_CONFIRM
			case ST_exit_confirm_open:
			{
				printf("Car %s: Leaves the parking lot.\n", msg->carName.c_str());

				CarMessage *carLeftLot = new CarMessage(ST_car_out, msg->carName);
				exit->send(carLeftLot);
					
				//Car does stuff outside parking lot
				sleep(rand() % 1 + 3);

				CarMessage *openReq = new CarMessage(ST_entry_request_open, carPTR, msg->carName);
				
				printf("Car %s: Requests Entry Open Door.\n", msg->carName.c_str());
				entry->send(openReq);
			
				break;
			}
			default:
				break;
		}
	}
	delete msg;
}