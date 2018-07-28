#ifndef VISHNU_DATASET_H
#define VISHNU_DATASET_H

#include <string>
#include <vector>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class DataSet;
  typedef std::shared_ptr< DataSet > DataSetPtr;
  typedef std::map< std::string, DataSetPtr > DataSets;

  class DataSet
  {

    public:

      DataSet( const std::string& path,
        const std::vector< std::string >& headers );

      std::string getPath( void ) const;

      void setPath( const std::string& path );

      std::vector< std::string > getHeaders( void ) const;

      void setHeaders( const std::vector< std::string >& headers );

    private:

      std::string _path;

      std::vector< std::string > _headers;

  };

}

#endif
