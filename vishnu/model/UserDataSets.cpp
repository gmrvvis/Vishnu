/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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