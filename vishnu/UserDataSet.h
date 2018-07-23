#ifndef VISHNU_DATASET_H
#define VISHNU_DATASET_H

#include <string>
#include <vector>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class DataSet
  {

    public:

      DataSet( const std::string& path, const bool& checked,
        const std::vector< std::string >& headers );

      std::string getPath( void ) const;

      void setPath( std::string path );

      bool getChecked( void ) const;

      void setChecked( bool checked );

      std::vector< std::string > getHeaders( void ) const;

      void setHeaders( const std::vector< std::string >& headers );

    private:

      std::string _path;

      bool _checked;

      std::vector< std::string > _headers;

  };

}

#endif
