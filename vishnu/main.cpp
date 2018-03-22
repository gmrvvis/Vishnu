#include <QApplication>
#include <QResource>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>

#include <QDesktopWidget>
#include <QStyle>

#include "MainWindow.h"
#include "Definitions.hpp"

#include <fstream>

#include <sp1common/sp1common.h>
#include <vishnu/version.h>
using namespace vishnu;

int main( int argc, char *argv[] )
{
  QApplication app( argc, argv );
  QApplication::setApplicationVersion( QString::fromStdString( 
    Version::getString( ) ) );
  QApplication::setApplicationName( APPLICATION_NAME );
  QApplication::setOrganizationName( ORGANIZATION_NAME );

  const QString sharedMemorykey = QApplication::applicationName( ) +
    QString( "-Key" );
  const QString semaphoreKey = QApplication::applicationName( ) +
    QString( "-SemKey" );

  QSystemSemaphore semaphore( semaphoreKey, 1 );
  semaphore.acquire();

  #ifdef __linux__
  // On unix, shared memory is not freed upon crash
  QSharedMemory unixSharedMemory( sharedMemorykey );
  //bool detach = false;
  if ( unixSharedMemory.attach( ) == false )
  {
    /*detach = */unixSharedMemory.detach( );
  }
  unixSharedMemory.detach( );
  #endif

  QSharedMemory sharedMemory( sharedMemorykey );
  bool running = sharedMemory.attach( );
  if ( !running )
  {
    sharedMemory.create( 1 );
  }

  semaphore.release( );

  if( running )
  {
    QMessageBox::warning( NULL, "Warning!", "Another instance of " +
      QApplication::applicationName( ) + " is already running!" );
    app.exit( );
    return 1;
  }

  sp1common::Args args( argc, argv );

  vishnu::MainWindow window( args );
  QResource::registerResource( "resources.rcc" );
  window.setGeometry( QRect( 0, 0, APPLICATION_WIDTH, APPLICATION_HEIGHT ) );

  QRect screenGeometry = QApplication::desktop()->screenGeometry();
  int x = ( screenGeometry.width() - window.width( ) ) / 2;
  int y = ( screenGeometry.height() - window.height( ) ) / 2;
  window.move( x, y );

  window.setMinimumSize( 300, 0 );
  window.setWindowIcon( QIcon( ":/icons/iconLauncher.png") );
  window.setIconSize( QSize( 32, 32 ) );
  window.setWindowTitle( QApplication::applicationName( ) );

  window.show( );
  return app.exec( );
}
