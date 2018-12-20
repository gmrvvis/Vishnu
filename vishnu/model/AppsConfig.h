/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_APPSCONFIG_H
#define VISHNU_APPSCONFIG_H

#include <QJsonObject>

#include <memory>

#include "Application.h"

#include <sp1common/sp1common.h>

namespace vishnu
{

  class AppsConfig;
  using AppsConfigPtr = std::shared_ptr< AppsConfig >;

  class AppsConfig
  {

    public:

      AppsConfig( );
      AppsConfig( const Applications& applications );
      ~AppsConfig( );

      Applications getApplications( void ) const;
      void setApplications( const Applications& applications );
      void addApplication( const ApplicationPtr& application );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      Applications _applications;
  };

}

#endif
