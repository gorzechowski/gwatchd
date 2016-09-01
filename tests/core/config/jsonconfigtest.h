#ifndef JSONCONFIGTEST_H
#define JSONCONFIGTEST_H

#include <QObject>

#include "../../../core/config/jsonconfig.h"

class JsonConfigTest: public QObject
{
    Q_OBJECT
public:
    JsonConfigTest(QObject *parent = 0);

protected:
    JsonConfig *m_config;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testValue();
    void testListValue();
    void testDefaultValue();
    void testMissingFile();
};

#endif // JSONCONFIGTEST_H
