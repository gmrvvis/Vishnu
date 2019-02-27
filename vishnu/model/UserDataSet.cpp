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

#include "UserDataSet.h"

namespace vishnu
{
  UserDataSet::UserDataSet( void )
  {

  }

  UserDataSet::UserDataSet( const std::string& name, const std::string& path,
    const std::string& csvFilename, const std::string& jsonFilename,
    const std::string& xmlFilename, const bool& selected )
      : _name( name )
      , _path( path )
      , _csvFilename( csvFilename )
      , _jsonFilename( jsonFilename )
      , _xmlFilename( xmlFilename )
      , _selected( selected )
  {

  }

  UserDataSet::~UserDataSet( void )
  {

  }

  //bool UserDataSet::operator==( const UserDataSet& other )


  std::string UserDataSet::getName( void ) const
  {
    return _name;
  }

  void UserDataSet::setName( const std::string& name )
  {
    _name = name;
  }


  std::string UserDataSet::getPath( void ) const
  {
    return _path;
  }

  void UserDataSet::setPath( const std::string& path )
  {
    _path = path;
  }

  std::string UserDataSet::getCsvFilename( void ) const
  {
    return _csvFilename;
  }

  void UserDataSet::setCsvFilename( const std::string& csvFilename )
  {
    _csvFilename = csvFilename;
  }

  std::string UserDataSet::getJsonFilename( void ) const
  {
    return _jsonFilename;
  }

  void UserDataSet::setJsonFilename( const std::string& jsonFilename )
  {
    _jsonFilename = jsonFilename;
  }

  std::string UserDataSet::getXmlFilename( void ) const
  {
    return _xmlFilename;
  }

  void UserDataSet::setXmlFilename( const std::string& xmlFilename )
  {
    _xmlFilename = xmlFilename;
  }

  bool UserDataSet::getSelected( void ) const
  {
    return _selected;
  }

  void UserDataSet::setSelected( const bool& selected )
  {
    _selected = selected;
  }

  void UserDataSet::deserialize( const QJsonObject &jsonObject )
  {
    _name = jsonObject[ "name" ].toString( ).toStdString( );
    _path = jsonObject[ "path" ].toString( ).toStdString( );
    _csvFilename = jsonObject[ "csvFilename" ].toString( ).toStdString( );
    _jsonFilename = jsonObject[ "jsonFilename" ].toString( ).toStdString( );
    _xmlFilename = jsonObject[ "xmlFilename" ].toString( ).toStdString( );
    _selected = jsonObject[ "selected" ].toBool( );
  }

  void UserDataSet::serialize( QJsonObject &jsonObject ) const
  {
    jsonObject[ "name" ] = QString::fromStdString( _name );
    jsonObject[ "path" ] = QString::fromStdString( _path );
    jsonObject[ "csvFilename" ] = QString::fromStdString( _csvFilename );
    jsonObject[ "jsonFilename" ] = QString::fromStdString( _jsonFilename );
    jsonObject[ "xmlFilename" ] = QString::fromStdString( _xmlFilename );
    jsonObject[ "selected" ] = _selected;
  }

}
