#include <QApplication>
#include <QResource>

#include "MainWindow.h"

int main( int argc, char *argv[] )
{
  QApplication a( argc, argv );
  vishnu::MainWindow w;

  QResource::registerResource( "resources.rcc" );
  w.setWindowTitle( QString::fromStdString( "Vishnu launcher" ) );

  w.show( );

  return a.exec( );
}
