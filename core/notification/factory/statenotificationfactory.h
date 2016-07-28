#ifndef STATENOTIFICATIONFACTORY_H
#define STATENOTIFICATIONFACTORY_H

#include "notification/statenotification.h"
#include "notification/payload.h"

class StateNotificationFactory
{
public:
    static StateNotification* create(QString jobName);
    static StateNotification* create(QString jobName, bool success);
    static StateNotification* create(QString jobName, Payload* payload);

protected:
    StateNotificationFactory();
};

#endif // STATENOTIFICATIONFACTORY_H
