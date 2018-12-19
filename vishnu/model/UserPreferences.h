/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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
