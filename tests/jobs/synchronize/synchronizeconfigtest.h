#ifndef SYNCHRONIZECONFIGTEST_H
#define SYNCHRONIZECONFIGTEST_H

#include <QObject>

#include "../../../jobs/synchronize/config/synchronizeconfig.h"
#include "config/jsonconfig.h"

class SynchronizeConfigTest : public QObject
{
    Q_OBJECT
public:
    SynchronizeConfigTest(QObject *parent = 0);

protected:
    SynchronizeConfig *m_config;

private slots:
    void initTestCase();

    void testValue();
    void testListValue();
    void testMissingFile();
};

#endif // SYNCHRONIZECONFIGTEST_H
