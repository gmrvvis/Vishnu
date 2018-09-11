/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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
