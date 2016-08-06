#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

#include "application/commandlineparser.h"
#include "config/config.h"

class Application: public QCoreApplication
{
public:
    Application(CommandLineParser *parser, int &argc, char **argv);

    enum Mode {
        Daemon = 0,
        Standard,
        Single
    };

    void init(Config *config);

    void removePidFile();

    QString configDir();

    bool isDaemon();
    bool isDebug();

protected:
    CommandLineParser *m_parser;

    int m_mode;

    void parseArguments();

    void unixSignalHandler();

    void initStandardMode(Config *config);
    void initSingleMode(Config *config);
    void initDaemonMode(Config *config);

    void showVersion();
    void showHelp();
    void showRunHelp();

};

#endif // APPLICATION_H
