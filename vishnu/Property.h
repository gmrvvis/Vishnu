#ifndef VISHNU_PROPERTY_H
#define VISHNU_PROPERTY_H

#include <string>
#include <vector>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class Property;
  typedef std::shared_ptr< Property > PropertyPtr;
  typedef std::vector< PropertyPtr > Properties;

  class Property
  {

    public:

      Property( const std::string& name, const bool& use,
        const bool& primaryKey, const sp1common::DataType& dataType,
        const sp1common::AxisType& axisType );

      std::string getName( void ) const;

      void setName( const std::string& name );

      bool getUse( void ) const;

      void setUse( const bool& use );

      bool getPrimaryKey( void ) const;

      void setPrimaryKey( const bool& primaryKey );

      sp1common::DataType getDataType( void ) const;

      void setDataType( const sp1common::DataType& dataType );

      sp1common::AxisType getAxisType( void ) const;

      void setAxisType( const sp1common::AxisType& axisType );

    private:

      std::string _name;

      bool _use;

      bool _primaryKey;

      sp1common::DataType _dataType;

      sp1common::AxisType _axisType;

  };

}

#endif
