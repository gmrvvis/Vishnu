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

#include "UserDataSets.h"

#include <QJsonArray>

namespace vishnu
{

  UserDataSets::UserDataSets( void )
  {

  }

  UserDataSets::UserDataSets( const UserDataSetVector& userDataSets )
    : _userDataSets( userDataSets )
  {

  }

  UserDataSets::~UserDataSets( void )
  {

  }

  UserDataSetVector UserDataSets::getUserDataSets( void ) const
  {
    return _userDataSets;
  }

  void UserDataSets::setUserDataSets( const UserDataSetVector& userDataSets )
  {
    _userDataSets = userDataSets;
  }

  void UserDataSets::addUserDataSet( const UserDataSetPtr& userDataSet )
  {
    _userDataSets.emplace_back( userDataSet );
  }

  void UserDataSets::deserialize( const QJsonObject &jsonObject )
  {
    QJsonArray userDataSets = jsonObject[ "userDataSets" ].toArray( );
    for (int i = 0; i < userDataSets.size(); ++i)
    {
      QJsonObject propertyObject = userDataSets.at( i ).toObject();
      UserDataSetPtr userDataSet( new UserDataSet( ) );
      userDataSet->deserialize( propertyObject );
      _userDataSets.emplace_back( userDataSet );
    }
  }

  void UserDataSets::serialize( QJsonObject &jsonObject ) const
  {
    QJsonArray userDataSets;
    for ( const auto& userDataSet : _userDataSets )
    {
        QJsonObject userDataSetObject;
        userDataSet->serialize( userDataSetObject );
        userDataSets.append( userDataSetObject );
    }
    jsonObject[ "userDataSets" ] = userDataSets;
  }

}
