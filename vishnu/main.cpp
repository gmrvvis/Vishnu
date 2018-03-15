#include <QApplication>
#include <QResource>
#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>

#include "MainWindow.h"
#include "Definitions.hpp"

#include <fstream>

#include <sp1common/Args.hpp>

int main( int argc, char *argv[] )
{
  QApplication app( argc, argv );
  QApplication::setApplicationVersion( "1.0" );
  QApplication::setApplicationName( "Launcher" );
  QApplication::setOrganizationName( "GMRV - URJC" );

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
  window.setWindowTitle( QApplication::applicationName( ) );
  window.show( );
  return app.exec( );
}
