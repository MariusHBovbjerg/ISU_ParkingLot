#include <time.h> /* time */
#include <stdlib.h>
#include "handlers.hpp"
#define MAX_QUEUE_SIZE 5

using namespace std;

struct ThreadQueues
{                        // Struct manages both entry & exit queues,
    MsgQueue entryQueue; // so they can be passed to car easily,
    MsgQueue exitQueue;  // as well as the entry and exit threads alike.
};

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
    ThreadQueues *queues = (ThreadQueues *)arg;
    MsgQueue *entryThreadQueue = &queues->entryQueue;
    MsgQueue *exitThreadQueue = &queues->exitQueue;
    MsgQueue carQueue(MAX_QUEUE_SIZE, "Car q");
    MsgQueue *carPTR = &carQueue;

    string name = gen_random(3);

    // Entry section, car sends open request to entryGuard
    // and handles incoming messsages.
    CarMessage *openReq = new CarMessage(ST_entry_request_open, carPTR, name);

    entryThreadQueue->send(openReq);
    printf("Car %s: Requests Entry Open Door.\n", name.c_str());

    for (;;)
    {
        CarMessage *entryMsg = (CarMessage*)carPTR->receive();

        carMsgHandler(entryMsg, entryThreadQueue, exitThreadQueue, carPTR);
    }
    return NULL;
}

int main(void)
{
    srand(time(NULL));
    int carCount = 0;
    pthread_t entryGuardThread;
    pthread_t exitGuardThread;
    pthread_t carSimulationThread[1024];

    MsgQueue entryMq(MAX_QUEUE_SIZE, "Entry q");
    MsgQueue exitMq(MAX_QUEUE_SIZE, "Exit q");

    ThreadQueues progQueues = {
        .entryQueue = entryMq,
        .exitQueue = exitMq
    };

    cout << "Create no. of cars: ";
    cin >> carCount;

    carSimulationThread[carCount];

    pthread_create(&entryGuardThread, nullptr, entryGuard, (void *)&progQueues.entryQueue);
    pthread_create(&exitGuardThread, nullptr, exitGuard, (void *)&progQueues.exitQueue);
    // pthread_create(&carSimulationThread, nullptr, carSimulation, NULL); Er den her nødvendig/gyldig når det er array?

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