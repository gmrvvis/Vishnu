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
