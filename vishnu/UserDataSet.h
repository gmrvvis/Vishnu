#ifndef VISHNU_USERDATASET_H
#define VISHNU_USERDATASET_H

#include <QString>
#include <QJsonObject>

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
      UserDataSet( const QString& name, const QString& path,
        const QString& csvFilename, const QString& xmlFilename,
        const bool& selected );
      ~UserDataSet( );

      QString getName( void ) const;
      void setName( const QString& name );

      QString getPath( void ) const;
      void setPath( const QString& path );

      QString getCsvFilename( void ) const;
      void setCsvFilename( const QString& csvFilename );

      QString getXmlFilename( void ) const;
      void setXmlFilename( const QString& xmlFilename );

      bool getSelected( void ) const;
      void setSelected( const bool& selected );

      void deserialize( const QJsonObject &jsonObject );
      void serialize( QJsonObject &jsonObject ) const;

    private:

      QString _name;
      QString _path;
      QString _csvFilename;
      QString _xmlFilename;
      bool _selected;
  };

}

#endif
