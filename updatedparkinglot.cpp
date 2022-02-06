#include <time.h>
#include <stdlib.h>
#include "handlers.hpp"
#define MAX_QUEUE_SIZE 5

using namespace std;

void *entryGuard(void *arg)
{
    MsgQueue *mq = (MsgQueue *)arg;
    for (;;)
    {
        CarMessage *msg = (CarMessage *)mq->receive();
        entryMsgHandler(msg);
    }
    return NULL;
}

void *exitGuard(void *arg)
{
    MsgQueue *mq = (MsgQueue *)arg;
    for (;;)
    {
        CarMessage *msg = (CarMessage *)mq->receive();
        exitMsgHandler(msg);
    }
    return NULL;
}

string gen_random(int len) {
    string s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

void *carSimulation(void *arg)
{
    string carName = gen_random(3);

    ThreadQueues *queues = (ThreadQueues *)arg;
    MsgQueue carQueue(MAX_QUEUE_SIZE, carName);

    CarMessage *openReq = new CarMessage(ST_entry_request_open, &carQueue, carName);

    queues->entryQueue->send(openReq);
    printf("Car %s: Requests Entry Open Door.\n", carName.c_str());

    for (;;)
    {
        CarMessage *entryMsg = (CarMessage*)carQueue.receive();

        carMsgHandler(entryMsg, queues, &carQueue);
    }
    return NULL;
}

int main(void)
{
    srand(time(NULL));
    int carCount;
    pthread_t entryGuardThread;
    pthread_t exitGuardThread;
    pthread_t carSimulationThread[1024];

    MsgQueue entryMq(MAX_QUEUE_SIZE, "Entry q");
    MsgQueue exitMq(MAX_QUEUE_SIZE, "Exit q");

    ThreadQueues progQueues = {
        .entryQueue = &entryMq,
        .exitQueue = &exitMq
    };

    printf("Create no. of cars:\n");

    scanf("%i", &carCount);

    carSimulationThread[carCount];

    pthread_create(&entryGuardThread, nullptr, entryGuard, (void *)progQueues.entryQueue);
    pthread_create(&exitGuardThread, nullptr, exitGuard, (void *)progQueues.exitQueue);

    for (int i = 0; i < carCount; i++)
    {
        pthread_create(&carSimulationThread[i], nullptr, carSimulation, (void *)(&progQueues));
    }

    for (int i = 0; i < carCount; i++)
    {
        pthread_join(carSimulationThread[i], NULL);
    }

    pthread_join(entryGuardThread, NULL);
    pthread_join(exitGuardThread, NULL);

    return 1;
}