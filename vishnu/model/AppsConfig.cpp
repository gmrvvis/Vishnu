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
