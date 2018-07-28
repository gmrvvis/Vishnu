#ifndef VISHNU_USERDATASET_H
#define VISHNU_USERDATASET_H

#include <QJsonObject>

#include <string>
#include <vector>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSet;
  typedef std::shared_ptr< UserDataSet > UserDataSetPtr;
  typedef std::vector< UserDataSetPtr > UserDataSetVector;

  class UserDataSet
  {

    public:

      UserDataSet( );
      UserDataSet( const std::string& name, const std::string& path,
        const std::string& csvFilename, const std::string& xmlFilename,
        const bool& selected );
      ~UserDataSet( );

      std::string getName( void ) const;
      void setName( const std::string& name );

      std::string getPath( void ) const;
      void setPath( const std::string& path );

      std::string getCsvFilename( void ) const;
      void setCsvFilename( const std::string& csvFilename );

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
      std::string _xmlFilename;
      bool _selected;
  };

}

#endif
