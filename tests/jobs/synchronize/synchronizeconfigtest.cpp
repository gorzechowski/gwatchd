#include <QTest>

#include "synchronizeconfigtest.h"

SynchronizeConfigTest::SynchronizeConfigTest(QObject *parent) : QObject(parent)
{

}

void SynchronizeConfigTest::initTestCase()
{
    this->m_config = new SynchronizeConfig(new JsonConfig(":/synchronize.json"));
}

void SynchronizeConfigTest::testValue()
{
    QCOMPARE(this->m_config->targetPath("/dir1/"), QString("/backup/dir1/"));
    QCOMPARE(this->m_config->targetPath("/dir2/"), QString("/backup/dir2/"));
    QCOMPARE(this->m_config->targetPath("/dir3/file.txt"), QString("/backup/dir3/file.txt"));
    QCOMPARE(this->m_config->targetUser("/dir1/"), QString("user"));
    QCOMPARE(this->m_config->targetUser("/dir2/"), QString("otherUser"));
    QCOMPARE(this->m_config->sshIdentityFile("/dir1/"), QString("/home/user/.ssh/id_rsa"));
    QCOMPARE(this->m_config->sshIdentityFile("/dir3/file.txt"), QString("/home/user/.ssh/service"));
    QCOMPARE(this->m_config->sshConfigFile("/dir1/"), QString("/home/user/sshConfig"));
    QCOMPARE(this->m_config->sshConfigFile("/dir3/file.txt"), QString("/home/user/.ssh/custom"));
    QCOMPARE(this->m_config->sshPort("/dir1/"), 22);
    QCOMPARE(this->m_config->sshPort("/dir2/"), 2222);
}

void SynchronizeConfigTest::testListValue()
{
    QCOMPARE(
        this->m_config->entries(),
        QStringList() << "/dir1/" << "/dir2/" << "/dir3/file.txt"
    );

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

void SynchronizeConfigTest::testMissingFile()
{
    SynchronizeConfig *config = new SynchronizeConfig(new JsonConfig("/not/existing/file"));

    QCOMPARE(config->value("undefined").toString("default"), QString("default"));
}
