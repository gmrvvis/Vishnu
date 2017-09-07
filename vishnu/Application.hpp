#ifndef APPLICATION_H
#define APPLICATION_H

#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <manco/manco.h>

namespace vishnu
{
  class Application : public QProcess
  {
    public:
      Application( const std::string& appName,
        const std::string& shellCommand, const std::map<std::string,
        std::string>& arguments = std::map<std::string, std::string>( ),
        const std::string& workingDirectory = std::string( ) );
      ~Application( );
      QPushButton* getPushButton( ) const;
      std::string getShellCommand( ) const;
      std::map<std::string, std::string> getArguments( ) const;
      void setArguments(
        const std::map<std::string, std::string>& arguments );
      void setArgument( const std::string& type, const std::string& value);

    private:
      std::string _shellCommand;
      std::map<std::string, std::string> _arguments;
      std::string _workingDirectory;
      QPushButton* _pushButton;
      QString capitalize( const QString &string ) const;
  };
}

#endif
