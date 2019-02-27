/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

#include <vishnucommon/vishnucommon.h>
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
    QMessageBox::warning( NULL, APPLICATION_NAME, "Another instance of " +
      QString::fromStdString( APPLICATION_NAME )+ " is already running!" );
    app.exit( );
    return 1;
  }

  QResource::registerResource( "resources.rcc" );

  vishnucommon::Args args( argc, argv );

  MainWindow window( args );
  window.setGeometry( QRect( 0, 0, APPLICATION_WIDTH, APPLICATION_HEIGHT ) );

  QRect screenGeometry = QApplication::desktop()->screenGeometry();
  int x = ( screenGeometry.width() - window.width( ) ) / 2;
  int y = ( screenGeometry.height() - window.height( ) ) / 2;
  window.move( x, y );

  window.setMinimumSize( 1000, 630 );
  window.setMaximumSize( 1000, 630 );
  window.setWindowIcon( QIcon( ":/icons/logoVishnu.png") );
  window.setIconSize( QSize( 32, 32 ) );
  window.setWindowTitle( QApplication::applicationName( ) );

  window.show( );
  return app.exec( );
}
