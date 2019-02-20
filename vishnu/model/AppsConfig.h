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

#ifndef VISHNU_APPSCONFIG_H
#define VISHNU_APPSCONFIG_H

#include <QJsonObject>

#include <memory>

#include "Application.h"

#include <vishnucommon/vishnucommon.h>

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
