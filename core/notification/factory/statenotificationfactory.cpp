#include "statenotificationfactory.h"

StateNotificationFactory::StateNotificationFactory()
{

}

StateNotification* StateNotificationFactory::create(QString jobName)
{
    StateNotification *notification = new StateNotification(
        jobName,
        StateNotification::Started
    );

    return notification;
}

StateNotification* StateNotificationFactory::create(QString jobName, bool success)
{
    StateNotification *notification = new StateNotification(
        jobName,
        success ? StateNotification::Finished : StateNotification::Failed
    );

    return notification;
}

StateNotification* StateNotificationFactory::create(QString jobName, Payload* payload)
{
    StateNotification *notification = new StateNotification(
        jobName,
        StateNotification::Running,
        payload
    );

    return notification;
}
