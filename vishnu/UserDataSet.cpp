#include "DataSet.h"

namespace vishnu
{
  DataSet::DataSet( const std::string& path, const bool& checked,
    const std::vector< std::string >& headers )
      : _path( path )
      , _checked( checked )
      , _headers( headers )
  {

  }

  std::string DataSet::getPath( void ) const
  {
    return _path;
  }

  void DataSet::setPath( std::string path )
  {
    _path = path;
  }

  bool DataSet::getChecked( void ) const
  {
    return _checked;
  }

  void DataSet::setChecked( bool checked )
  {
    _checked = checked;
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
