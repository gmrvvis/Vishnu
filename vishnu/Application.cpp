#include "Application.hpp"

namespace vishnu
{
  Application::Application( const std::string& appName, const std::string& shellCommand,
                            const std::map<std::string, std::string>& arguments,
                            const std::string& workingDirectory )
  {
    _pushButton = new QPushButton( capitalize( QString::fromStdString( appName ) ) );
    _pushButton->setEnabled( false );

    _shellCommand = shellCommand;

    /* Not needed
    #if defined(_WIN32) || defined(WIN32)
      _shellCommand += ".exe";
    #endif
    */

    _arguments = arguments;

    _workingDirectory = workingDirectory;
    if ( workingDirectory != "" )
    {
      this->setWorkingDirectory( QString::fromStdString( workingDirectory ) );
    }
  }

  Application::~Application( )
  {

  }

  QPushButton* Application::getPushButton( ) const
  {
    return _pushButton;
  }

  QString Application::capitalize( const QString &string ) const
  {
    QString tmp = string;
    tmp = tmp.toLower( );
    tmp[0] = string[0].toUpper( );
    return tmp;
  }
  std::string Application::getShellCommand( ) const
  {
    return _shellCommand;
  }

  std::map<std::string, std::string> Application::getArguments( ) const
  {
    return _arguments;
  }

  void Application::setArguments( const std::map<std::string, std::string>& arguments )
  {
    _arguments = arguments;
  }

  void Application::setArgument( const std::string& type, const std::string& value)
  {
    _arguments[type] = value;
  }
}
