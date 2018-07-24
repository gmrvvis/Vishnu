#ifndef VISHNU_USERDATASETS_H
#define VISHNU_USERDATASETS_H

#include <QJsonObject>

#include <string>
#include <vector>

#include "UserDataSet.h"

#include <sp1common/sp1common.h>

namespace vishnu
{

  typedef std::shared_ptr< UserDataSet > UserDataSetPtr;
  typedef std::vector< UserDataSetPtr > UserDataSetVector;

  class UserDataSets;
  typedef std::unique_ptr< UserDataSets > UserDataSetsPtr;

  class UserDataSets
  {

    public:

      UserDataSets( );
      UserDataSets( const UserDataSetVector& userDataSets );
      ~UserDataSets( );

      UserDataSetVector getUserDataSets( void ) const;
      void setUserDataSets( const UserDataSetVector& userDataSets );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      UserDataSetVector _userDataSets;
  };

}

#endif
