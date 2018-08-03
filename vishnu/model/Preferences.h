/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_PREFERENCES_H
#define VISHNU_PREFERENCES_H

#include <QJsonObject>

#include <string>
#include <map>
#include <memory>

namespace vishnu
{

  class Preferences;
  typedef std::shared_ptr< Preferences > PreferencesPtr;

  typedef std::map< std::string, std::string > PreferencesMap;

  class Preferences
  {

    public:

      Preferences( void );

      Preferences( const PreferencesMap& preferences );

      ~Preferences( void );

      PreferencesMap getPreferences( void ) const;
      void setPreferences( const PreferencesMap& preferences );
      void addPreference( const std::string& key, const std::string& value );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      PreferencesMap _preferences;

  };

}

#endif
