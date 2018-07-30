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
    std::string defaultPath =
      QStandardPaths::writableLocation(
        QStandardPaths::HomeLocation ).toStdString( );
    std::string defaultCsvFilename = DEFAULT_DATASET_FILENAME
      + std::string( "." ) + STR_EXT_CSV;
    std::string defaultJsonFilename = DEFAULT_DATASET_FILENAME
      + std::string( "." ) + STR_EXT_JSON;
    std::string defaultXmlFilename = DEFAULT_DATASET_FILENAME
      + std::string( "." ) + STR_EXT_XML;

    setName( DEFAULT_DATASET_FILENAME );
    setPath( defaultPath );
    setBrowsePath( );
    setCsvFilename( defaultCsvFilename );
    setJsonFilename( defaultJsonFilename );
    setXmlFilename( defaultXmlFilename );

    QLabel* _nameLabel = new QLabel( "Name:" );
    QLabel* _pathLabel = new QLabel( "Path:" );
    QLabel* _csvFilenameLabel = new QLabel( "CSV Filename:" );
    QLabel* _jsonFilenameLabel = new QLabel( "JSON Filename:" );
    QLabel* _xmlFilenameLabel = new QLabel( "XML Filename:" );

    QGridLayout* mainLayout = new QGridLayout( this );

    mainLayout->addWidget( _nameLabel, 0, 0, 1, 1, Qt::AlignRight );
    mainLayout->addWidget( _name, 0, 1 );

    mainLayout->addWidget( _pathLabel, 1, 0, 1, 1, Qt::AlignRight );
    mainLayout->addWidget( _path, 1, 1 );
    mainLayout->addWidget( _browsePath, 1, 2 );

    mainLayout->addWidget( _csvFilenameLabel, 2, 0, 1, 1, Qt::AlignRight );
    mainLayout->addWidget( _csvFilename, 2, 1 );

    mainLayout->addWidget( _jsonFilenameLabel, 3, 0, 1, 1, Qt::AlignRight );
    mainLayout->addWidget( _jsonFilename, 3, 1 );

    mainLayout->addWidget( _xmlFilenameLabel, 4, 0, 1, 1, Qt::AlignRight );
    mainLayout->addWidget( _xmlFilename, 4, 1 );

    setLayout( mainLayout );
  }

  PathsWidget::~PathsWidget( void )
  {

  }

  std::string PathsWidget::getName( void ) const
  {
    return _name->text( ).toStdString( );
  }

  void PathsWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new QLineEdit( );
    }
    _name->setText( QString::fromStdString( name ) );
  }

  std::string PathsWidget::getPath( void ) const
  {
    return _path->text( ).toStdString( );
  }

  void PathsWidget::setPath( const std::string& path )
  {
    if ( _path == nullptr)
    {
      _path = new QLineEdit( );
    }
    _path->setText( QString::fromStdString( path ) );
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

  std::string PathsWidget::getCsvFilename( void ) const
  {
    return _csvFilename->text( ).toStdString( );
  }

  void PathsWidget::setCsvFilename( const std::string& csvFilename )
  {
    if ( _csvFilename == nullptr)
    {
      _csvFilename = new QLineEdit( );
    }
    _csvFilename->setText( QString::fromStdString( csvFilename ) );
  }

  std::string PathsWidget::getJsonFilename( void ) const
  {
    return _jsonFilename->text( ).toStdString( );
  }

  void PathsWidget::setJsonFilename( const std::string& jsonFilename )
  {
    if ( _jsonFilename == nullptr)
    {
      _jsonFilename = new QLineEdit( );
    }
    _jsonFilename->setText( QString::fromStdString( jsonFilename ) );
  }

  std::string PathsWidget::getXmlFilename( void ) const
  {
    return _xmlFilename->text( ).toStdString( );
  }

  void PathsWidget::setXmlFilename( const std::string& xmlFilename )
  {
    if ( _xmlFilename == nullptr)
    {
      _xmlFilename = new QLineEdit( );
    }
    _xmlFilename->setText( QString::fromStdString( xmlFilename ) );
  }

  UserDataSetPtr PathsWidget::getUserDataSet( void )
  {
    UserDataSetPtr userDataSet( new UserDataSet( _name->text( ).toStdString( ),
      _path->text( ).toStdString( ), _csvFilename->text( ).toStdString( ),
      _jsonFilename->text( ).toStdString( ),
      _xmlFilename->text( ).toStdString( ), false ) );
    return userDataSet;
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
