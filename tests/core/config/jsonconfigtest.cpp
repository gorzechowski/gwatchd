#include <QTest>
#include <QDebug>

#include "jsonconfigtest.h"

JsonConfigTest::JsonConfigTest(QObject *parent) : QObject(parent)
{

}

void JsonConfigTest::initTestCase()
{
    this->m_config = new JsonConfig(":/synchronize.json");
}

void JsonConfigTest::cleanupTestCase()
{

}

void JsonConfigTest::testValue()
{
    QCOMPARE(this->m_config->value("delay").toInt(), 100);

    QCOMPARE(this->m_config->value("ssh").toObject().value("identityFile").toString(), QString("/home/user/.ssh/id_rsa"));

    QCOMPARE(this->m_config->value("ssh").toObject().value("port").toInt(), 22);
    QCOMPARE(this->m_config->value("dirs").toObject().value("/dir2/").toObject().value("ssh").toObject().value("port").toInt(), 2222);

    QCOMPARE(this->m_config->value("target").toObject().value("user").toString(), QString("user"));
    QCOMPARE(this->m_config->value("dirs").toObject().value("/dir2/").toObject().value("target").toObject().value("user").toString(), QString("otherUser"));

    QCOMPARE(this->m_config->targetPath("/dir1/"), QString("/backup/dir1/"));
    QCOMPARE(this->m_config->targetPath("/dir2/"), QString("/backup/dir2/"));
    QCOMPARE(this->m_config->targetPath("/dir3/file.txt"), QString("/backup/dir3/file.txt"));

    QStringList entries = this->m_config->entries();

    QCOMPARE(
        entries.contains("/dir1/") && entries.contains("/dir2/") && entries.contains("/dir3/file.txt"),
        true
    );
}

void JsonConfigTest::testListValue()
{
    QCOMPARE(
        this->m_config->excludes("/dir1/"),
        QStringList() << "*.git" << "*.local"
    );

    QCOMPARE(
        this->m_config->excludes("/dir2/"),
        QStringList()
    );

    QCOMPARE(
        this->m_config->includes("/dir1/"),
        QStringList() << "important.local" << "important2.local"
    );

    QCOMPARE(
        this->m_config->includes("/dir2/"),
        QStringList()
    );

    QCOMPARE(
        this->m_config->targetHosts(),
        QStringList() << "127.0.0.1"
    );

    QCOMPARE(
        this->m_config->sshOptions(),
        QStringList() << "StrictHostKeyChecking=no"
    );
}

void JsonConfigTest::testDefaultValue()
{
    QCOMPARE(this->m_config->value("undefined", "default").toString(), QString("default"));
}

void JsonConfigTest::testMissingFile()
{
    JsonConfig *config = new JsonConfig("/not/existing/file");

    QCOMPARE(config->value("undefined", "default").toString(), QString("default"));
}
