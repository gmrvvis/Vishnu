#include "UserDataSet.h"

namespace vishnu
{
  UserDataSet::UserDataSet( )
  {

  }

  UserDataSet::UserDataSet( const std::string& name, const std::string& csvPath,
    const std::string& xmlPath, const bool& selected )
      : _name( name )
      , _csvPath( csvPath )
      , _xmlPath( xmlPath )
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

  std::string UserDataSet::getCsvPath( void ) const
  {
    return _csvPath;
  }

  void UserDataSet::setCsvPath( const std::string& csvPath )
  {
    _csvPath = csvPath;
  }

  std::string UserDataSet::getXmlPath( void ) const
  {
    return _xmlPath;
  }

  void UserDataSet::setXmlPath( const std::string& xmlPath )
  {
    _xmlPath = xmlPath;
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
    _csvPath = jsonObject[ "csvPath" ].toString( ).toStdString( );
    _xmlPath = jsonObject[ "xmlPath" ].toString( ).toStdString( );
    _selected = jsonObject[ "selected" ].toBool( );
  }

  void UserDataSet::serialize( QJsonObject &jsonObject ) const
  {
    jsonObject[ "name" ] = QString::fromStdString( _name );
    jsonObject[ "csvPath" ] = QString::fromStdString( _csvPath );
    jsonObject[ "xmlPath" ] = QString::fromStdString( _xmlPath );
    jsonObject[ "selected" ] = _selected;
  }

}
