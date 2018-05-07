#ifndef VISHNU_APPLICATION_H
#define VISHNU_APPLICATION_H

#include <QProcess>
#include <QPushButton>
#include <QStringList>

#include <sp1common/sp1common.h>

namespace vishnu
{
  class Application : public QProcess
  {
    public:
      Application( const sp1common::ApplicationType& applicationType,
        const std::string& displayName,
        const std::string& shellCommand, 
        const sp1common::Args& args,
        const QIcon icon,
        const std::string& workingDirectory = std::string( ) );
      ~Application( );

      sp1common::ApplicationType getApplicationType( ) const;
      std::string getDisplayName( ) const;
      std::string getCmd( ) const;
      sp1common::ArgsMap getArgs( ) const;
      QPushButton* getPushButton( ) const;

    private:
      
      sp1common::ApplicationType _applicationType;
      std::string _displayName;
      std::string _cmd;
      sp1common::Args _args;
      std::string _workingDirectory;
      QPushButton* _pushButton;
  };
}

#endif
