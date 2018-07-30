#ifndef VISHNU_PATHSWIDGET_H
#define VISHNU_PATHSWIDGET_H

#include <QPushButton>
#include <QLineEdit>

#include <string>
#include <memory>

#include "../UserDataSet.h"

#include <sp1common/sp1common.h>

namespace vishnu
{

  class PathsWidget;
  typedef std::shared_ptr< PathsWidget > PathsWidgetPtr;

  class PathsWidget : public QWidget
  {

    Q_OBJECT

    public:

      PathsWidget( void );
      ~PathsWidget( void );

      std::string getName( void ) const;
      std::string getPath( void ) const;
      std::string getJsonFilename( void ) const;
      std::string getCsvFilename( void ) const;
      std::string getXmlFilename( void ) const;

      UserDataSetPtr getUserDataSet( void );

    private slots:
      void slotBrowsePath( void );

    private:
      QLineEdit* _name = nullptr;
      QLineEdit* _path = nullptr;
      QPushButton* _browsePath = nullptr;
      QLineEdit* _csvFilename = nullptr;
      QLineEdit* _jsonFilename = nullptr;
      QLineEdit* _xmlFilename = nullptr;

      void setName( const std::string& name );
      void setPath( const std::string& path );
      void setBrowsePath( void );
      void setCsvFilename( const std::string& csvFilename );
      void setJsonFilename( const std::string& jsonFilename );
      void setXmlFilename( const std::string& xmlFilename );

  };

}

#endif
