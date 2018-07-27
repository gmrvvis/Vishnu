#include "UserDataSet.h"

namespace vishnu
{
  UserDataSet::UserDataSet( )
  {

  }

  UserDataSet::UserDataSet( const QString& name, const QString& path,
    const QString& csvFilename, const QString& xmlFilename,
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

  QString UserDataSet::getName( void ) const
  {
    return _name;
  }

  void UserDataSet::setName( const QString& name )
  {
    _name = name;
  }


  QString UserDataSet::getPath( void ) const
  {
    return _path;
  }

  void UserDataSet::setPath( const QString& path )
  {
    _path = path;
  }

  QString UserDataSet::getCsvFilename( void ) const
  {
    return _csvFilename;
  }

  void UserDataSet::setCsvFilename( const QString& csvFilename )
  {
    _csvFilename = csvFilename;
  }

  QString UserDataSet::getXmlFilename( void ) const
  {
    return _xmlFilename;
  }

  void UserDataSet::setXmlFilename( const QString& xmlFilename )
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
    _name = jsonObject[ "name" ].toString( );
    _path = jsonObject[ "path" ].toString( );
    _csvFilename = jsonObject[ "csvFilename" ].toString( );
    _xmlFilename = jsonObject[ "xmlFilename" ].toString( );
    _selected = jsonObject[ "selected" ].toBool( );
  }

  void UserDataSet::serialize( QJsonObject &jsonObject ) const
  {
    jsonObject[ "name" ] = _name;
    jsonObject[ "path" ] = _path;
    jsonObject[ "csvFilename" ] = _csvFilename;
    jsonObject[ "xmlFilename" ] = _xmlFilename;
    jsonObject[ "selected" ] = _selected;
  }

}
