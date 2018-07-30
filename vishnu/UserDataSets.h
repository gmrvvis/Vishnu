#ifndef VISHNU_USERDATASETS_H
#define VISHNU_USERDATASETS_H

#include <QJsonObject>

#include <memory>

#include "UserDataSet.h"

#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSets;
  typedef std::shared_ptr< UserDataSets > UserDataSetsPtr;

  class UserDataSets
  {

    public:

      UserDataSets( );
      UserDataSets( const UserDataSetVector& userDataSets );
      ~UserDataSets( );

      UserDataSetVector getUserDataSets( void ) const;
      void setUserDataSets( const UserDataSetVector& userDataSets );
      void addUserDataSet( const UserDataSetPtr& userDataSet );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      UserDataSetVector _userDataSets;
  };

}

#endif
