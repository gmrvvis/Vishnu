#ifndef VISHNU_USERDATASET_H
#define VISHNU_USERDATASET_H

#include <QJsonObject>

#include <string>
#include <vector>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSet
  {

    public:

      UserDataSet( );
      UserDataSet( const std::string& name, const std::string& csvPath,
        const std::string& xmlPath, const bool& selected );
      ~UserDataSet( );

      std::string getName( void ) const;
      void setName( const std::string& name );

      std::string getCsvPath( void ) const;
      void setCsvPath( const std::string& csvPath );

      std::string getXmlPath( void ) const;
      void setXmlPath( const std::string& xmlPath );

      bool getSelected( void ) const;
      void setSelected( const bool& selected );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      std::string _name;
      std::string _csvPath;
      std::string _xmlPath;
      bool _selected;
  };

}

#endif
