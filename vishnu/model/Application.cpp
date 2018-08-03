/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#include "Application.h"

namespace vishnu
{
  Application::Application( )
  {

  }

  Application::Application( const sp1common::ApplicationType& applicationType,
    const std::string& displayName, const std::string& shellCommand,
    const sp1common::Args& args, const std::string& workingDirectory,
    const std::string& iconPath )
      : _applicationType( applicationType )
      , _displayName( displayName )
      , _shellCommand( shellCommand )
      , _args( args )
      , _workingDirectory( workingDirectory )
      , _iconPath( iconPath )
  {

  }

  Application::~Application( )
  {

  }

  sp1common::ApplicationType Application::getApplicationType( void ) const
  {
    return _applicationType;
  }

  void Application::setApplicationType( const sp1common::ApplicationType& applicationType )
  {
    _applicationType = applicationType;
  }

  std::string Application::getDisplayName( void ) const
  {
    return _displayName;
  }

  void Application::setDisplayName( const std::string& displayName )
  {
    _displayName = displayName;
  }

  std::string Application::getShellCommand( void ) const
  {
    return _shellCommand;
  }

  void Application::setShellCommand( const std::string& shellCommand )
  {
    _shellCommand = shellCommand;
  }

  sp1common::Args Application::getArgs( void ) const
  {
    return _args;
  }

  void Application::setArgs( const sp1common::Args& args )
  {
    _args = args;
  }

  std::string Application::getWorkingDirectory( void ) const
  {
    return _workingDirectory;
  }

  void Application::setWorkingDirectory( const std::string& workingDirectory )
  {
    _workingDirectory = workingDirectory;
  }

  std::string Application::getIconPath( void ) const
  {
    return _iconPath;
  }

  void Application::setIconPath( const std::string& iconPath )
  {
    _iconPath = iconPath;
  }

  void Application::deserialize( const QJsonObject &jsonObject )
  {
    _applicationType = sp1common::toApplicationType(
      jsonObject[ "applicationType" ].toString( ).toStdString( ) );
    _displayName = jsonObject[ "displayName" ].toString( ).toStdString( );
    _shellCommand = jsonObject[ "shellCommand" ].toString( ).toStdString( );
    _args.deserialize( jsonObject );
    _workingDirectory =
      jsonObject[ "workingDirectory" ].toString( ).toStdString( );
    _iconPath = jsonObject[ "iconPath" ].toString( ).toStdString( );
  }

  void Application::serialize( QJsonObject &jsonObject ) const
  {
    jsonObject[ "applicationType" ] = QString::fromStdString( toString( _applicationType ) );
    jsonObject[ "displayName" ] = QString::fromStdString( _displayName );
    jsonObject[ "shellCommand" ] = QString::fromStdString( _shellCommand );
    _args.serialize( jsonObject );
    jsonObject[ "workingDirectory" ] =
      QString::fromStdString( _workingDirectory );
    jsonObject[ "iconPath" ] = QString::fromStdString( _iconPath );
  }

}
