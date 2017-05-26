#ifndef APPLICATION_H
#define APPLICATION_H

#include <QPushButton>
#include <QStringList>
#include <manco/manco.h>
#include "Process.hpp"

namespace vishnu
{
    class Application
    {
    public:
        Application(const std::string& appName );
        ~Application( );
        std::vector<Process*> getProcesses( ) const;
        void addProcess(const std::string& shellCommand ,
          const QStringList& arguments );
        QPushButton* getPushButton( ) const;

    private:
        std::vector<Process*> _processes;
        QPushButton* _pushButton;
        QString capitalize( const QString &string ) const;
    };
}

#endif
