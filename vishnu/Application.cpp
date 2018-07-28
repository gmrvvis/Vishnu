#include "Application.h"

namespace vishnu
{
  Application::Application( const sp1common::ApplicationType& applicationType,
    const std::string& displayName, const std::string& cmd,
    const sp1common::Args& args, const QIcon icon,
    const std::string& workingDirectory )
      : _applicationType( applicationType )
     , _displayName( displayName )
      , _cmd( cmd )
      , _args( args )
      , _workingDirectory( workingDirectory )
  {
    _pushButton = new QPushButton( QString::fromStdString( displayName ) );
    _pushButton->setStyleSheet(
      "text-align:left; font-weight: bold; font-size: 18px" );
    _pushButton->setIcon( icon );
    _pushButton->setIconSize( QSize( 64,64 ) );
    _pushButton->setSizePolicy( QSizePolicy::Preferred,
      QSizePolicy::Preferred );
    //_pushButton->setEnabled( false );
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
