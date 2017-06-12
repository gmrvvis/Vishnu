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
          const std::map<std::string, std::string>& arguments,
          const std::string& workingDirectory);
        ~Process( );
        std::string getShellCommand( ) const;
        std::map<std::string, std::string> getArguments( ) const;
        void setArguments( const std::map<std::string, std::string>& arguments );
        void setArgument( const std::string& type, const std::string& value);

    private:
        std::string _shellCommand;
        std::map<std::string, std::string> _arguments;
        std::string _workingDirectory;
    };
}

#endif
