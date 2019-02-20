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

#include "AppsConfig.h"

#include <QJsonArray>

namespace vishnu
{

  AppsConfig::AppsConfig( )
  {

  }

  AppsConfig::AppsConfig( const Applications& applications )
    : _applications( applications )
  {

  }

  AppsConfig::~AppsConfig( )
  {

  }

  Applications AppsConfig::getApplications( void ) const
  {
    return _applications;
  }

  void AppsConfig::setApplications( const Applications& applications )
  {
    _applications = applications;
  }

  void AppsConfig::addApplication( const ApplicationPtr& application )
  {
    _applications.emplace_back( application );
  }

  void AppsConfig::deserialize( const QJsonObject &jsonObject )
  {
    QJsonArray applications = jsonObject[ "applications" ].toArray( );
    for (int i = 0; i < applications.size(); ++i)
    {
      QJsonObject applicationObject = applications.at( i ).toObject();
      ApplicationPtr application( new Application( ) );
      application->deserialize( applicationObject );
      _applications.emplace_back( application );
    }
  }

  void AppsConfig::serialize( QJsonObject &jsonObject ) const
  {
    QJsonArray applications;
    for ( const auto& application : _applications )
    {
        QJsonObject applicationObject;
        application->serialize( applicationObject );
        applications.append( applicationObject );
    }
    jsonObject[ "applications" ] = applications;
  }

}
