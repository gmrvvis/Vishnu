#ifndef APPLICATION_H
#define APPLICATION_H

#include <QPushButton>
#include <QProcess>

namespace vishnu
{
    class Application : public QProcess
    {
    public:
        Application( std::string appName, std::string appPath);
        ~Application( );
        std::string getLauncherCmd( );
        QPushButton* getPushButton( );

    private:
        std::string _launcherCmd;
        QPushButton* _pushButton;
        QString capitalize( const QString &string );
    };
}

#endif
