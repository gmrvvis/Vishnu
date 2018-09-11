#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>

#include <string>

#include <sp1common/sp1common.h>

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
  if ( sp1common::JSON::serialize( userPreferencesFile, userPreferences ) )
  {
    std::cout << "User Preferences file created sucessfully." << std::endl;
  }
  else
  {
    std::cout << "Error creating User Preferences file." << std::endl;
  }

  //Read User Preferences file
  UserPreferencesPtr readedPreferencesFile
    = sp1common::JSON::deserialize< UserPreferences >( userPreferencesFile );

  return 0;
}
