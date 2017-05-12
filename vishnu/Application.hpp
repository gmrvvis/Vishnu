#ifndef APPLICATION_H
#define APPLICATION_H

#include <QPushButton>
#include <QProcess>
#include <manco/manco.h>

namespace vishnu
{
    class Application : public QProcess
    {
    public:
        Application( const std::string& appName, const std::string& shellExecPath );
        ~Application( );
        std::string getShellExecPath( ) const;
        QPushButton* getPushButton( ) const;

    private:
        std::string _shellExecPath;
        QPushButton* _pushButton;
        QString capitalize( const QString &string ) const;
    };
}

#endif
