#ifndef NOTIFIER
#define NOTIFIER

#include "notification/notification.h"

class Notifier
{
public:
    virtual ~Notifier() {}

    virtual bool notify(Notification*) = 0;
};

#endif // NOTIFIER

