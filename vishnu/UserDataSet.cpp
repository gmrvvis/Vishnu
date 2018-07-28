#include "UserDataSet.h"

namespace vishnu
{
  UserDataSet::UserDataSet( )
  {

  }

  UserDataSet::UserDataSet( const std::string& name, const std::string& path,
    const std::string& csvFilename, const std::string& xmlFilename,
    const bool& selected )
      : _name( name )
      , _path( path )
      , _csvFilename( csvFilename )
      , _xmlFilename( xmlFilename )
      , _selected( selected )
  {

  }

  UserDataSet::~UserDataSet( )
  {

  }

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
    _xmlFilename = jsonObject[ "xmlFilename" ].toString( ).toStdString( );
    _selected = jsonObject[ "selected" ].toBool( );
  }

  void UserDataSet::serialize( QJsonObject &jsonObject ) const
  {
    jsonObject[ "name" ] = QString::fromStdString( _name );
    jsonObject[ "path" ] = QString::fromStdString( _path );
    jsonObject[ "csvFilename" ] = QString::fromStdString( _csvFilename );
    jsonObject[ "xmlFilename" ] = QString::fromStdString( _xmlFilename );
    jsonObject[ "selected" ] = _selected;
  }

}
