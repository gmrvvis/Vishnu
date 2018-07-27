#ifndef VISHNU_PATHSWIDGET_H
#define VISHNU_PATHSWIDGET_H

#include <QString>
#include <QPushButton>
#include <QLineEdit>

#include <memory>

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

      QString getPath( void ) const;
      QString getCsvFilename( void ) const;
      QString getXmlFilename( void ) const;

    private slots:
      void slotBrowsePath( void );

    private:
      QLineEdit* _path = nullptr;
      QPushButton* _browsePath = nullptr;
      QLineEdit* _csvFilename = nullptr;
      QLineEdit* _xmlFilename = nullptr;

      void setPath( const QString& path );
      void setBrowsePath( void );
      void setCsvFilename( const QString& csvFilename );
      void setXmlFilename( const QString& xmlFilename );

  };

}

#endif
