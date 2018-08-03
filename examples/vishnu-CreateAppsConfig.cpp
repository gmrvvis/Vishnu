#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>

#include <string>

#include <sp1common/sp1common.h>

#include "../vishnu/Definitions.hpp"
#include "../vishnu/model/AppsConfig.h"

using namespace vishnu;

int main( int argc, char* argv[] )
{
  QApplication app( argc, argv );

  std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
      + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );

  QString qUserDataFolder = QString::fromStdString( userDataFolder );
  QDir qDir( qUserDataFolder );
  if ( !qDir.exists( ) )
  {
    qDir.mkpath( qUserDataFolder );
  }

  std::string appsConfigFile = userDataFolder + FILE_APPS_CONFIG;

  AppsConfigPtr appsConfig( new AppsConfig( ) );

  sp1common::Args clintArgs;
  clintArgs.set( "-z", "hbp://" );
  clintArgs.set( "-sp", 31400 );
  clintArgs.set( "-ce" );
  clintArgs.set( "-ch", "http://localhost" );
  clintArgs.set( "-cp", 8765 );

  ApplicationPtr clintApp( new Application(
    sp1common::ApplicationType::CLINT, "Clint", "ClintExplorer", clintArgs,
    "", ":/icons/logoClint.png" ) );

  appsConfig->addApplication( clintApp );

  sp1common::Args dcExplorerArgs;
  dcExplorerArgs.set( "-z", "hbp://" );
  dcExplorerArgs.set( "-p", 12345 );
  dcExplorerArgs.set( "-u", "http://localhost:8888" );
  dcExplorerArgs.set( "-n", "DCExplorer" );
  dcExplorerArgs.set( "-dcn", "apicolat" );

  ApplicationPtr dcExplorerApp( new Application(
    sp1common::ApplicationType::DCEXPLORER, "DCExplorer", "WeCo",
    dcExplorerArgs, "", ":/icons/logoDCExplorer.png" ) );

  appsConfig->addApplication( dcExplorerApp );

  sp1common::Args pyramidalArgs;
  pyramidalArgs.set( "-z", "hbp://" );

  ApplicationPtr pyramidalApp( new Application(
    sp1common::ApplicationType::PYRAMIDAL, "Pyramidal", "CellExplorer",
    pyramidalArgs, "", ":/icons/logoPyramidal.png" ) );

  appsConfig->addApplication( pyramidalApp );

  //Create Apps Config file
  if ( sp1common::JSON::serialize( appsConfigFile, appsConfig ) )
  {
    std::cout << "Apps Config file created sucessfully." << std::endl;
  }
  else
  {
    std::cout << "Error creating Apps Config file." << std::endl;
  }

  //Read Apps Config file
  AppsConfigPtr readedAppsConfigFile
    = sp1common::JSON::deserialize< AppsConfig >( appsConfigFile );

  return 0;
}
