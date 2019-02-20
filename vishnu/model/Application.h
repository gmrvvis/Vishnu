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

#ifndef VISHNU_APPLICATION_H
#define VISHNU_APPLICATION_H

#include <QJsonObject>

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class Application;
  using ApplicationPtr = std::shared_ptr< Application >;
  using Applications = std::vector< ApplicationPtr >;

  class Application
  {

    public:

      Application( );
      Application( const vishnucommon::ApplicationType& applicationType,
        const std::string& displayName, const std::string& shellCommand,
        const vishnucommon::Args& args, const std::string& workingDirectory,
        const std::string& iconPath );
      ~Application( );

      vishnucommon::ApplicationType getApplicationType( void ) const;
      void setApplicationType(
        const vishnucommon::ApplicationType& applicationType );

      std::string getDisplayName( void ) const;
      void setDisplayName( const std::string& displayName );

      std::string getShellCommand( void ) const;
      void setShellCommand( const std::string& shellCommand );

      vishnucommon::Args getArgs( void ) const;
      void setArgs( const vishnucommon::Args& args );

      std::string getWorkingDirectory( void ) const;
      void setWorkingDirectory( const std::string& workingDirectory );

      std::string getIconPath( void ) const;
      void setIconPath( const std::string& iconPath );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      vishnucommon::ApplicationType _applicationType;
      std::string _displayName;
      std::string _shellCommand;
      vishnucommon::Args _args;
      std::string _workingDirectory;
      std::string _iconPath;
  };

}

#endif
