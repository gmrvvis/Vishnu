#include "Application.hpp"

namespace vishnu
{
  Application::Application( const std::string& appName )
  {
    _pushButton = new QPushButton( capitalize( QString::fromStdString( appName ) ) );
    _pushButton->setEnabled( false );
  }

  Application::~Application( )
  {

  }

  std::vector<Process*> Application::getProcesses( ) const
  {
    return _processes;
  }

  QPushButton* Application::getPushButton( ) const
  {
    return _pushButton;
  }

  void Application::addProcess( const std::string& shellCommand,
    const std::map<std::string, std::string>& arguments,
    const std::string& workingDirectory )
  {
    _processes.push_back( new Process( shellCommand, arguments, workingDirectory ) );
  }
    
  QString Application::capitalize( const QString &string ) const
  {
    QString tmp = string;
    tmp = tmp.toLower( );
    tmp[0] = string[0].toUpper( );
    return tmp;
  }
}
