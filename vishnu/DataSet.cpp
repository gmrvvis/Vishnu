#include "DataSet.h"

namespace vishnu
{
  DataSet::DataSet( const std::string& path,
    const std::vector< std::string >& headers )
      : _path( path )
      , _headers( headers )
  {

  }

  std::string DataSet::getPath( void ) const
  {
    return _path;
  }

  void DataSet::setPath( const std::string& path )
  {
    _path = path;
  }

  std::vector< std::string > DataSet::getHeaders( void ) const
  {
    return _headers;
  }

  void DataSet::setHeaders( const std::vector< std::string >& headers )
  {
    _headers = headers;
  }

}
