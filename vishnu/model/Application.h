/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_APPLICATION_H
#define VISHNU_APPLICATION_H

#include <QJsonObject>

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class Application;
  typedef std::shared_ptr< Application > ApplicationPtr;
  typedef std::vector< ApplicationPtr > Applications;

  class Application
  {

    public:

      Application( );
      Application( const sp1common::ApplicationType& applicationType,
        const std::string& displayName, const std::string& shellCommand,
        const sp1common::Args& args, const std::string& workingDirectory,
        const std::string& iconPath );
      ~Application( );

      sp1common::ApplicationType getApplicationType( void ) const;
      void setApplicationType(
        const sp1common::ApplicationType& applicationType );

      std::string getDisplayName( void ) const;
      void setDisplayName( const std::string& displayName );

      std::string getShellCommand( void ) const;
      void setShellCommand( const std::string& shellCommand );

      sp1common::Args getArgs( void ) const;
      void setArgs( const sp1common::Args& args );

      std::string getWorkingDirectory( void ) const;
      void setWorkingDirectory( const std::string& workingDirectory );

      std::string getIconPath( void ) const;
      void setIconPath( const std::string& iconPath );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      sp1common::ApplicationType _applicationType;
      std::string _displayName;
      std::string _shellCommand;
      sp1common::Args _args;
      std::string _workingDirectory;
      std::string _iconPath;
  };

}

#endif
