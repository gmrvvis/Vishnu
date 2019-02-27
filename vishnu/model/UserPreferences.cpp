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

#include "UserPreferences.h"

#include <QStringList>

namespace vishnu
{
  UserPreferences::UserPreferences( void )
  {

  }

  UserPreferences::UserPreferences( const UserPreferencesMap& userPreferences )
    : _userPreferences( userPreferences )
  {

  }

  UserPreferences::~UserPreferences( void )
  {

  }

  UserPreferencesMap UserPreferences::getUserPreferences( void ) const
  {
    return _userPreferences;
  }

  std::string UserPreferences::getUserPreference( const std::string& key ) const
  {
    auto it = _userPreferences.find( key );
    if ( it != _userPreferences.end( ) )
    {
      return it->second;
    }
    return std::string( );
  }

  void UserPreferences::setUserPreferences( const UserPreferencesMap& userPreferences )
  {
    _userPreferences = userPreferences;
  }

  void UserPreferences::addUserPreference( const std::string& key,
    const std::string& value )
  {
    _userPreferences[ key ] = value;
  }

  void UserPreferences::deserialize( const QJsonObject &jsonObject )
  {
    QJsonObject userPreferences = jsonObject[ "userPreferences" ].toObject( );
    for (int i = 0; i < userPreferences.size( ); ++i)
    {
      std::string key = userPreferences.keys( ).at( i ).toStdString( );
      _userPreferences[ key ] =
        userPreferences[ QString::fromStdString( key ) ]
          .toString( ).toStdString( );
    }
  }

  void UserPreferences::serialize( QJsonObject &jsonObject ) const
  {
    QJsonObject userPreferences;
    for ( const auto& userPreference : _userPreferences )
    {
      userPreferences[ QString::fromStdString( userPreference.first ) ] =
        QString::fromStdString( userPreference.second );
    }
    jsonObject[ "userPreferences" ] = userPreferences;
  }
}
