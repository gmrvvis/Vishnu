/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>

#include <string>

#include <vishnucommon/vishnucommon.h>

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

  vishnucommon::Args clintArgs;
  clintArgs.set( "-z", "hbp://" );
  clintArgs.set( "-sp", 31400 );
  clintArgs.set( "-ce" );
  clintArgs.set( "-ch", "http://localhost" );
  clintArgs.set( "-cp", 8765 );

  ApplicationPtr clintApp( new Application(
    vishnucommon::ApplicationType::CLINT, "Clint", "ClintExplorer", clintArgs,
    "", ":/icons/logoClint.png" ) );

  appsConfig->addApplication( clintApp );

  vishnucommon::Args dcExplorerArgs;
  dcExplorerArgs.set( "-z", "hbp://" );
  dcExplorerArgs.set( "-p", 12345 );
  dcExplorerArgs.set( "-u", "http://localhost:8888" );
  dcExplorerArgs.set( "-n", "DCExplorer" );
  dcExplorerArgs.set( "-dcn", "apicolat" );

  ApplicationPtr dcExplorerApp( new Application(
    vishnucommon::ApplicationType::DCEXPLORER, "DCExplorer", "WeCo",
    dcExplorerArgs, "", ":/icons/logoDCExplorer.png" ) );

  appsConfig->addApplication( dcExplorerApp );

  vishnucommon::Args pyramidalArgs;
  pyramidalArgs.set( "-z", "hbp://" );

  ApplicationPtr pyramidalApp( new Application(
    vishnucommon::ApplicationType::PYRAMIDAL, "Pyramidal", "CellExplorer",
    pyramidalArgs, "", ":/icons/logoPyramidal.png" ) );

  appsConfig->addApplication( pyramidalApp );

  //Create Apps Config file
  if ( vishnucommon::JSON::serialize( appsConfigFile, appsConfig ) )
  {
    std::cout << "Apps Config file created sucessfully." << std::endl;
  }
  else
  {
    std::cout << "Error creating Apps Config file." << std::endl;
  }

  //Read Apps Config file
  AppsConfigPtr readedAppsConfigFile
    = vishnucommon::JSON::deserialize< AppsConfig >( appsConfigFile );

  return 0;
}
