/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#include "Preferences.h"

#include <QStringList>

namespace vishnu
{
  Preferences::Preferences( void )
  {

  }

  Preferences::Preferences( const PreferencesMap& preferences )
    : _preferences( preferences )
  {

  }

  Preferences::~Preferences( void )
  {

  }

  PreferencesMap Preferences::getPreferences( void ) const
  {
    return _preferences;
  }

  void Preferences::setPreferences( const PreferencesMap& preferences )
  {
    _preferences = preferences;
  }

  void Preferences::addPreference( const std::string& key,
    const std::string& value )
  {
    _preferences[ key ] = value;
  }

  void Preferences::deserialize( const QJsonObject &jsonObject )
  {
    QJsonObject preferences = jsonObject[ "preferences" ].toObject( );
    for (int i = 0; i < preferences.size( ); ++i)
    {
      std::string key = preferences.keys( ).at( i ).toStdString( );
      _preferences[ key ] =
        preferences[ QString::fromStdString( key ) ].toString( ).toStdString( );
    }
  }

  void Preferences::serialize( QJsonObject &jsonObject ) const
  {
    QJsonObject preferences;
    for ( const auto& preference : _preferences )
    {
      preferences[ QString::fromStdString( preference.first ) ] =
        QString::fromStdString( preference.second );
    }
    jsonObject[ "preferences" ] = preferences;
  }
}
