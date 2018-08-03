#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>

#include <string>

#include <sp1common/sp1common.h>

#include "../vishnu/Definitions.hpp"
#include "../vishnu/model/Preferences.h"

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

  std::string preferencesFile = userDataFolder + FILE_USER_PREFERENCES;

  PreferencesPtr preferences( new Preferences( ) );

  preferences->addPreference( "zeqSession", "hbp://" );
  preferences->addPreference( "workingDirectory", "" );

  //Create Preferences file
  if ( sp1common::JSON::serialize( preferencesFile, preferences ) )
  {
    std::cout << "Apps Config file created sucessfully." << std::endl;
  }
  else
  {
    std::cout << "Error creating Apps Config file." << std::endl;
  }

  //Read Preferences file
  PreferencesPtr readedPreferencesFile
    = sp1common::JSON::deserialize< Preferences >( preferencesFile );

  return 0;
}
