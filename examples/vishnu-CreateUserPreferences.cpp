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
#include "../vishnu/model/UserPreferences.h"

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

  std::string userPreferencesFile = userDataFolder + FILE_USER_PREFERENCES;

  UserPreferencesPtr userPreferences( new UserPreferences( ) );

  userPreferences->addUserPreference( "zeqSession", "hbp://" );
  userPreferences->addUserPreference( "workingDirectory", "" );

  //Create User Preferences file
  if ( vishnucommon::JSON::serialize( userPreferencesFile, userPreferences ) )
  {
    std::cout << "User Preferences file created sucessfully." << std::endl;
  }
  else
  {
    std::cout << "Error creating User Preferences file." << std::endl;
  }

  //Read User Preferences file
  UserPreferencesPtr readedPreferencesFile
    = vishnucommon::JSON::deserialize< UserPreferences >( userPreferencesFile );

  return 0;
}
