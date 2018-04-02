#include "Application.h"

namespace vishnu
{
  Application::Application( const sp1common::ApplicationType& applicationType,
    const std::string& displayName, const std::string& cmd,
    const sp1common::Args& args, const std::string& workingDirectory )
    : _applicationType( applicationType )
    , _displayName( displayName )
    , _cmd( cmd )
    , _args( args )
    , _workingDirectory( workingDirectory )
  {
    _pushButton = new QPushButton( QString::fromStdString( displayName ) );
    _pushButton->setEnabled( false );

    /* Not needed
    #if defined(_WIN32) || defined(WIN32)
      _shellCommand += ".exe";
    #endif
    */
  }

  Application::~Application( )
  {

  }

  sp1common::ApplicationType Application::getApplicationType( ) const
  {
    return _applicationType;
  }

  std::string Application::getDisplayName( ) const
  {
    return _displayName;
  }

  std::string Application::getCmd( ) const
  {
    return _cmd;
  }

  sp1common::ArgsMap Application::getArgs( ) const
  {
    return _args.get( );
  }

  QPushButton* Application::getPushButton( ) const
  {
    return _pushButton;
  }
}
