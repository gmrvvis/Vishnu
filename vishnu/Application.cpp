#include "Application.h"

namespace vishnu
{
    Application::Application( std::string appName, std::string appPath )
    {
        QString qName = QString::fromStdString( appName.c_str( ) );
        _launcherCmd = appPath;

        #ifdef defined(_WIN32) || defined(WIN32)
            _launcherCmd += ".exe";
        #endif

      _pushButton = new QPushButton( capitalize( qName ) );
      _pushButton->setObjectName( qName );
      _pushButton->setEnabled( false );

    }

    Application::~Application( )
    {

    }

    std::string Application::getLauncherCmd( )
    {
        return _launcherCmd;
    }

    QPushButton* Application::getPushButton( )
    {
        return _pushButton;
    }

    QString Application::capitalize( const QString &string )
    {
        QString tmp = string;
        tmp = tmp.toLower( );
        tmp[0] = string[0].toUpper( );
        return tmp;
    }
}
