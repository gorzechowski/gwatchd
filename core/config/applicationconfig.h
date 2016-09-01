#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QObject>
#include <QFileInfo>

#include "config/config.h"

class ApplicationConfig : public QObject
{
    Q_OBJECT
public:
    ApplicationConfig(Config *config, QObject *parent = 0);

    QFileInfo fileInfo();

    QString logsDirPath();
    int logsMaxFileSize();

    QString socketAddress();
    int socketPort();

protected:
    Config *m_config;
};

#endif // APPLICATIONCONFIG_H
