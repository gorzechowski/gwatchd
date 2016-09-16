#ifndef JOBCONFIG_H
#define JOBCONFIG_H

#include <QString>

#include "config/config.h"

class JobConfig
{
public:
    JobConfig(Config *config): m_config(config), m_context("dirs") {}

protected:
    Config *m_config;

    QString m_context;
};

#endif // JOBCONFIG_H
