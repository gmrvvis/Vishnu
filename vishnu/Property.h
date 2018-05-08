#ifndef VISHNU_PROPERTY_H
#define VISHNU_PROPERTY_H

#include <string>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class Property
  {

    public:

      Property( const std::string& name, const bool& use, const bool&primaryKey,
        const sp1common::DataType& dataType );

      std::string getName( void ) const;

      void setName( const std::string& name );

      bool getUse( void ) const;

      void setUse( const bool& use );

      bool getPrimaryKey( void ) const;

      void setPrimaryKey( const bool& primaryKey );

      sp1common::DataType getDataType( void ) const;

      void setDataType( const sp1common::DataType& dataType );

    private:

      std::string _name;

      bool _use;

      bool _primaryKey;

      sp1common::DataType _dataType;

  };

}

#endif
