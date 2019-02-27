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
