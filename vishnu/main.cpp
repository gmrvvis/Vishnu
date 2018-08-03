/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

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
  if ( unixSharedMemory.attach( ) == false )
  {
    unixSharedMemory.detach( );
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

  QResource::registerResource( "resources.rcc" );

  sp1common::Args args( argc, argv );

  MainWindow window( args );
  window.setGeometry( QRect( 0, 0, APPLICATION_WIDTH, APPLICATION_HEIGHT ) );

  QRect screenGeometry = QApplication::desktop()->screenGeometry();
  int x = ( screenGeometry.width() - window.width( ) ) / 2;
  int y = ( screenGeometry.height() - window.height( ) ) / 2;
  window.move( x, y );

  window.setMinimumSize( 1000, 500 );
  window.setMaximumSize( 1000, 500 );
  window.setWindowIcon( QIcon( ":/icons/logoVishnu.png") );
  window.setIconSize( QSize( 32, 32 ) );
  window.setWindowTitle( QApplication::applicationName( ) );

  window.show( );
  return app.exec( );
}
