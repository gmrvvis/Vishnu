#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>
#include <QStringList>
#include <iostream>

namespace vishnu
{
    class Process : public QProcess
    {
    public:
        Process( const std::string& shellCommand,
          const QStringList& arguments );
        ~Process( );
        std::string getShellCommand( ) const;
        QStringList getArguments( ) const;
        void setArguments( const QStringList& arguments );
        void addArgument( const std::string& argument );

    private:
        std::string _shellCommand;
        QStringList _arguments;
    };
}

#endif
