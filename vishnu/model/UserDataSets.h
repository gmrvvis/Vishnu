/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VISHNU_USERDATASETS_H
#define VISHNU_USERDATASETS_H

#include <QJsonObject>

#include <memory>

#include "UserDataSet.h"

#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class UserDataSets;
  using UserDataSetsPtr = std::shared_ptr< UserDataSets >;

  class UserDataSets
  {

    public:

      UserDataSets( void );
      UserDataSets( const UserDataSetVector& userDataSets );
      ~UserDataSets( void );

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
