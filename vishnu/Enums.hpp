#ifndef VISHNU_ENUMS_HPP
#define VISHNU_ENUMS_HPP

#include <sp1common/sp1common.h>
#include "Definitions.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace vishnu
{
  enum class AxisType
  {
    None,
    X,
    Y,
    Z,
    XYZ
  };

  inline static AxisType toAxisType( const std::string& axisType )
  {
    std::string upperAxisType = sp1common::Strings::upper( axisType );
    if ( upperAxisType == STR_EMPTY )
    {
      return AxisType::None;
    }
    else if ( upperAxisType == STR_X )
    {
      return AxisType::X;
    }
    else if ( upperAxisType == STR_Y )
    {
      return AxisType::Y;
    }
    else if ( upperAxisType == STR_Z )
    {
      return AxisType::Z;
    }
    else if ( upperAxisType == STR_XYZ )
    {
      return AxisType::XYZ;
    }
    else
    {
      assert( 0 && "Error: Invalid AxisType enum constant!" );
      exit( -1 );
    }
  }

  inline static std::string toString( const AxisType& axisType )
  {
    std::string result;
    switch ( axisType )
    {
      case AxisType::None:
        result = STR_EMPTY;
        break;
      case AxisType::X:
        result = STR_X;
        break;
      case AxisType::Y:
        result = STR_Y;
        break;
      case AxisType::Z:
        result = STR_Z;
        break;
      case AxisType::XYZ:
        result = STR_XYZ;
        break;
    }
    return result;
  }

  inline static std::vector<std::string> axisTypesToVector( )
  {
    std::vector<std::string> vector;
    vector.push_back( toString( AxisType::None ) );
    vector.push_back( toString( AxisType::X ) );
    vector.push_back( toString( AxisType::Y ) );
    vector.push_back( toString( AxisType::Z ) );
    vector.push_back( toString( AxisType::XYZ ) );
    return vector;
  }
}

#endif