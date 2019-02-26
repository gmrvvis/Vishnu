/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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

#ifndef VISHNU_USERPREFERENCES_H
#define VISHNU_USERPREFERENCES_H

#include <QJsonObject>

#include <string>
#include <map>
#include <memory>

namespace vishnu
{

  class UserPreferences;
  using UserPreferencesPtr = std::shared_ptr< UserPreferences >;
  using UserPreferencesMap = std::map< std::string, std::string >;

  class UserPreferences
  {

    public:

      UserPreferences( void );

      UserPreferences( const UserPreferencesMap& userPreferences );

      ~UserPreferences( void );

      UserPreferencesMap getUserPreferences( void ) const;
      std::string getUserPreference( const std::string& key ) const;
      void setUserPreferences( const UserPreferencesMap& userPreferences );
      void addUserPreference( const std::string& key, const std::string& value );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      UserPreferencesMap _userPreferences;

  };

}

#endif
