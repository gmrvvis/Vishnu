#ifndef VISHNU_APPLICATION_HPP
#define VISHNU_APPLICATION_HPP

#include <QProcess>
#include <QPushButton>
#include <QStringList>

#include <manco/manco.h>
#include <sp1common/sp1common.h>

namespace vishnu
{
  class Application : public QProcess
  {
    public:
      Application( const manco::ApplicationType& applicationType,
        const std::string& displayName,
        const std::string& shellCommand, 
        const sp1common::Args& args,
        const std::string& workingDirectory = std::string( ) );
      ~Application( );

      manco::ApplicationType getApplicationType( ) const;
      std::string getDisplayName( ) const;
      std::string getCmd( ) const;
      sp1common::ArgsMap getArgs( ) const;
      QPushButton* getPushButton( ) const;

    private:
      
      manco::ApplicationType _applicationType;
      std::string _displayName;
      std::string _cmd;
      sp1common::Args _args;
      std::string _workingDirectory;
      QPushButton* _pushButton;
  };
}

#endif
