/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
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
