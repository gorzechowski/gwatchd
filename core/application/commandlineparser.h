#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <QCommandLineOption>

class CommandLineParser : public QCommandLineParser
{
public:
    CommandLineParser();

    QString pidFile();
    QString configDir();
    QString runJobName();
    QStringList runJobArgs();

    bool isSetRun();
    bool isSetNoDaemon();
    bool isSetPidFile();
    bool isSetConfigDir();
    bool isSetHelp();
    bool isSetVersion();
    bool isSetDebug();

protected:
    QCommandLineOption m_noDaemon;
    QCommandLineOption m_pidFile;
    QCommandLineOption m_configDir;
    QCommandLineOption m_help;
    QCommandLineOption m_version;
    QCommandLineOption m_debug;
};

#endif // COMMANDLINEPARSER_H
