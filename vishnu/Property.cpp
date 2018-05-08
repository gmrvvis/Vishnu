#include "Property.h"

namespace vishnu
{
  Property::Property( const std::string& name, const bool& use,
    const bool& primaryKey, const sp1common::DataType& dataType )
      : _name( name )
      , _use( use )
      , _primaryKey( primaryKey )
      , _dataType( dataType )
  {

  }

  std::string Property::getName( void ) const
  {
    return _name;
  }

  void Property::setName( const std::string& name )
  {
    _name = name;
  }

  bool Property::getUse( void ) const
  {
    return _use;
  }

  void Property::setUse( const bool& use )
  {
    _use = use;
  }

  bool Property::getPrimaryKey( void ) const
  {
    return _primaryKey;
  }

  void Property::setPrimaryKey( const bool& primaryKey )
  {
    _primaryKey = primaryKey;
  }

  sp1common::DataType Property::getDataType( void ) const
  {
    return _dataType;
  }

  void Property::setDataType( const sp1common::DataType& dataType )
  {
    _dataType = dataType;
  }
}
