/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_USERDATASET_H
#define VISHNU_USERDATASET_H

#include <QJsonObject>

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSet;
  using UserDataSetPtr = std::shared_ptr< UserDataSet >;
  using UserDataSetVector = std::vector< UserDataSetPtr >;
  using UserDataSetMap = std::map< std::string, UserDataSetPtr >;

  class UserDataSet
  {

    public:

      UserDataSet( void );
      UserDataSet( const std::string& name, const std::string& path,
        const std::string& csvFilename, const std::string& jsonFilename,
        const std::string& xmlFilename, const bool& selected );
      ~UserDataSet( void );
      bool operator==( const UserDataSet& other )
      {
          std::cout << "comp" <<std::endl;
          return (
            ( _name == other.getName( ) )
            && ( _path == other.getPath( ) )
            && ( _csvFilename == other.getCsvFilename( ) )
            && ( _jsonFilename == other.getJsonFilename( ) )
            && ( _xmlFilename == other.getXmlFilename( ) )
          );
      }

      std::string getName( void ) const;
      void setName( const std::string& name );

      std::string getPath( void ) const;
      void setPath( const std::string& path );

      std::string getCsvFilename( void ) const;
      void setCsvFilename( const std::string& csvFilename );

      std::string getJsonFilename( void ) const;
      void setJsonFilename( const std::string& jsonFilename );

      std::string getXmlFilename( void ) const;
      void setXmlFilename( const std::string& xmlFilename );

      bool getSelected( void ) const;
      void setSelected( const bool& selected );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      std::string _name;
      std::string _path;
      std::string _csvFilename;
      std::string _jsonFilename;
      std::string _xmlFilename;
      bool _selected;
  };

}

#endif
