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
