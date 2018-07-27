#include "PathsWidget.h"

#include <QLabel>
#include <QStandardPaths>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../Definitions.hpp"

namespace vishnu
{

  PathsWidget::PathsWidget( void )
  {
    QString defaultPath =
      QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
    QString defaultCsvFilename = DEFAULT_DATASET_FILENAME + QString( "." )
      + STR_EXT_CSV;
    QString defaultXmlFilename = DEFAULT_DATASET_FILENAME + QString( "." )
      + STR_EXT_XML;

    setPath( defaultPath );
    setBrowsePath( );
    setCsvFilename( defaultCsvFilename );
    setXmlFilename( defaultXmlFilename );

    QLabel* _pathLabel = new QLabel( "Path:" );
    QLabel* _csvFilenameLabel = new QLabel( "CSV Filename:" );
    QLabel* _xmlFilenameLabel = new QLabel( "XML Filename:" );

    QVBoxLayout* mainVLayout = new QVBoxLayout( this );

    QHBoxLayout* pathHLayout = new QHBoxLayout( );
    pathHLayout->addWidget( _pathLabel, 0 );
    pathHLayout->addSpacing( 90 );
    pathHLayout->addWidget( _path, 1 );
    pathHLayout->addSpacing( 30 );
    pathHLayout->addWidget( _browsePath, 2, Qt::AlignRight );
    mainVLayout->addLayout( pathHLayout, 0 );

    QHBoxLayout* csvHLayout = new QHBoxLayout( );
    csvHLayout->addWidget( _csvFilenameLabel, 0 );
    csvHLayout->addSpacing( 30 );
    csvHLayout->addWidget( _csvFilename, 1 );
    mainVLayout->addLayout( csvHLayout, 0 );

    QHBoxLayout* xmlHLayout = new QHBoxLayout( );
    xmlHLayout->addWidget( _xmlFilenameLabel, 0 );
    xmlHLayout->addSpacing( 30 );
    xmlHLayout->addWidget( _xmlFilename, 1 );
    mainVLayout->addLayout( xmlHLayout, 0 );

    setLayout( mainVLayout );
  }

  PathsWidget::~PathsWidget( void )
  {

  }

  QString PathsWidget::getPath( void ) const
  {
    return _path->text( );
  }

  void PathsWidget::setPath( const QString& path )
  {
    if ( _path == nullptr)
    {
      _path = new QLineEdit( );
    }
    _path->setText( path );
  }

  void PathsWidget::setBrowsePath( void )
  {
    if ( _browsePath == nullptr)
    {
      _browsePath = new QPushButton( );
    }
    _browsePath->setToolTip( "Browse Path");
    _browsePath->setIconSize( QSize( 32, 32 ) );
    _browsePath->setIcon( QIcon( ":/icons/browseFolder.png" ) );

    QObject::connect( _browsePath, SIGNAL( clicked( ) ), this,
      SLOT( slotBrowsePath( ) ) );
  }

  QString PathsWidget::getCsvFilename( void ) const
  {
    return _csvFilename->text( );
  }

  void PathsWidget::setCsvFilename( const QString& csvFilename )
  {
    if ( _csvFilename == nullptr)
    {
      _csvFilename = new QLineEdit( );
    }
    _csvFilename->setText( csvFilename );
  }

  QString PathsWidget::getXmlFilename( void ) const
  {
    return _xmlFilename->text( );
  }

  void PathsWidget::setXmlFilename( const QString& xmlFilename )
  {
    if ( _xmlFilename == nullptr)
    {
      _xmlFilename = new QLineEdit( );
    }
    _xmlFilename->setText( xmlFilename );
  }

  void PathsWidget::slotBrowsePath( void )
  {
    QString defPath =
      QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
    QString dir = QFileDialog::getExistingDirectory( this, tr( "Browse Path" ),
      defPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if ( !dir.isEmpty( ) )
    {
      _path->setText( dir );
    }
  }

}
