/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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

#include "Application.h"

namespace vishnu
{
  Application::Application( )
  {

  }

  Application::Application( const vishnucommon::ApplicationType& applicationType,
    const std::string& displayName, const std::string& shellCommand,
    const vishnucommon::Args& args, const std::string& workingDirectory,
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

  vishnucommon::ApplicationType Application::getApplicationType( void ) const
  {
    return _applicationType;
  }

  void Application::setApplicationType( const vishnucommon::ApplicationType& applicationType )
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

  vishnucommon::Args Application::getArgs( void ) const
  {
    return _args;
  }

  void Application::setArgs( const vishnucommon::Args& args )
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
    _applicationType = vishnucommon::toApplicationType(
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
