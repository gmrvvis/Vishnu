#include <QApplication>
#include <QResource>
//#include <QMessageBox>
//#include <QSharedMemory>

#include "MainWindow.h"

int main( int argc, char *argv[] )
{
  QApplication app( argc, argv );
  QApplication::setApplicationVersion( "1.0" );
  QApplication::setApplicationName( "Launcher" );
  QApplication::setOrganizationName( "GMRV - URJC" );

  /*QSharedMemory sharedMemory;
  sharedMemory.setKey( QApplication::applicationName() + "-Key" );
  if( sharedMemory.create( 1 ) == false )
  {
    QMessageBox::warning( NULL, "Warning!", "Another instance of " +
      QApplication::applicationName( ) + " is already running!" );
    app.exit( );
    return 0;
  }*/

  vishnu::MainWindow window;
  QResource::registerResource( "resources.rcc" );
  window.setWindowTitle( QApplication::applicationName( ) );
  window.show( );
  return app.exec( );
}
