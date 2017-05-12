#include "Application.hpp"

namespace vishnu
{
    Application::Application( const std::string& appName, const std::string& shellExecPath )
    {
        QString qName = QString::fromStdString( appName );
        _shellExecPath = shellExecPath;

        #ifdef defined(_WIN32) || defined(WIN32)
            _launcherCmd += ".exe";
        #endif

      _pushButton = new QPushButton( capitalize( qName ) );
      //_pushButton->setObjectName( qName );
      _pushButton->setEnabled( false );

    }

    Application::~Application( )
    {

    }

    std::string Application::getShellExecPath( ) const
    {
        return _shellExecPath;
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
}
