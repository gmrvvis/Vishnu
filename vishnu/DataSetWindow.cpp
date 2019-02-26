/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "DataSetWindow.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>

#include <map>

#include "Definitions.hpp"

namespace vishnu
{

  DataSetWindow::DataSetWindow( QWidget* parent )
    : QDialog( parent )
  {
    //ToolBar
    _toolBar = new QToolBar( );
    _toolBar->setMovable( false );
    _toolBar->setIconSize( QSize( 32, 32 ) );

    QToolButton* addFileButton = new QToolButton( );
    addFileButton->setIconSize( QSize( 32, 32 ) );
    addFileButton->setIcon( QIcon( ":/icons/addFile.png" ) );
    addFileButton->setText( "Add File(s)" );
    addFileButton->setToolTip( "Add File(s)" );
    addFileButton->setShortcut( QKeySequence( "Ctrl+O" ) );
    addFileButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    _toolBar->addWidget( addFileButton );

    QObject::connect( addFileButton, SIGNAL( clicked( ) ), this,
      SLOT( slotAddFiles( ) ) );

    //Widget Layout
    QVBoxLayout* widgetVBoxLayout = new QVBoxLayout();
    setLayout( widgetVBoxLayout );

    //PathsWidget
    _pathsWidget.reset( new PathsWidget( ) );

    //DataSetListWidget
    _dataSetListWidget.reset( new DataSetListWidget( ) );

    qRegisterMetaType< std::vector< std::string > >("StringVector");

    QObject::connect( _dataSetListWidget.get( ),
      SIGNAL( signalAddFiles( const std::vector< std::string >& ) ), this,
      SLOT( slotAddFiles( const std::vector< std::string >& ) ) );

    //PropertiesTableWidget
    _propertiesTableWidget.reset( new PropertiesTableWidget( ) );

    //Buttons
    _cancelButton = new QPushButton("Cancel", this);
    QObject::connect( _cancelButton, SIGNAL( clicked( ) ), this,
      SLOT( slotCancelButton( ) ) );
    _createButton = new QPushButton("Create", this);
    QObject::connect( _createButton, SIGNAL( clicked( ) ), this,
      SLOT( slotCreateButton( ) ) );

    QHBoxLayout* buttonsHBoxLayout = new QHBoxLayout( );
    buttonsHBoxLayout->addStretch( 255 );
    buttonsHBoxLayout->addWidget( _cancelButton, 0, Qt::AlignRight );
    buttonsHBoxLayout->addWidget( _createButton, 0, Qt::AlignRight );

    //Fill layout with toolbar, datasets, properties and buttons
    widgetVBoxLayout->addWidget( _pathsWidget.get( ), 0 );
    widgetVBoxLayout->addWidget( _toolBar, 1 );
    widgetVBoxLayout->addWidget( _dataSetListWidget.get( ), 2 );
    widgetVBoxLayout->addWidget( _propertiesTableWidget.get( ), 3 );
    widgetVBoxLayout->addLayout( buttonsHBoxLayout );
  }

  DataSetWindow::~DataSetWindow()
  {

  }

  UserDataSetPtr DataSetWindow::getResultUserDataSet( void )
  {
    return _pathsWidget->getUserDataSet( );
  }

  void DataSetWindow::slotAddFiles(
    const std::vector< std::string >& dropped )
  {
    DataSetWidgets dataSetWidgets =
      _dataSetListWidget->addDataSets( dropped );
    vishnucommon::PropertyGroupsPtr propertyGroups =
      _dataSetListWidget->getPropertyGroups( );

    for ( const auto& dataSetWidget : dataSetWidgets )
    {
      QObject::connect( dataSetWidget, SIGNAL( removeSelected( ) ), this,
        SLOT( slotRemoveDataSet( ) ) );

      _propertiesTableWidget->addProperties(
         dataSetWidget->getDataSet( )->getProperties( ), propertyGroups );

      _propertiesTableWidget->checkPrimaryKeys(
        _dataSetListWidget->getCommonProperties( ) );
    }
  }

  void DataSetWindow::slotRemoveDataSet( )
  {
    std::vector< std::string > propertiesToRemove =
      _dataSetListWidget->getPropertiesToRemove( );

    _propertiesTableWidget->removeProperties( propertiesToRemove );

    _dataSetListWidget->removeCurrentDataSet( );

    _propertiesTableWidget->checkPrimaryKeys(
      _dataSetListWidget->getCommonProperties( ) );
  }

  void DataSetWindow::slotCreateButton( void )
  {
    //TODO: Check for userdataset name (unique?)
    //std::string name = _pathsWidget->getName( );

    //Create path
    std::string path = _pathsWidget->getPath( );
    QString qPath = QString::fromStdString( path );
    QDir qDir( qPath );

    std::string csvPath = qDir.absolutePath( ).toStdString( )
      + std::string( "/" ) + _pathsWidget->getCsvFilename( );

    std::string jsonPath = qDir.absolutePath( ).toStdString( )
      + std::string( "/" ) + _pathsWidget->getJsonFilename( );

    std::string xmlPath = qDir.absolutePath( ).toStdString( )
      + std::string( "/" ) + _pathsWidget->getXmlFilename( );

    if ( !qDir.exists( ) )
    {
      if ( !qDir.mkpath( qPath ) )
      {
        vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
          "Can't create " + path + " folder.", false );
        return;
      }
    }
    else
    {
      if ( vishnucommon::Files::exist( csvPath ) )
      {
        QMessageBox::StandardButton overwriteCsv = QMessageBox::warning( this,
          "CSV File exists", QString::fromStdString( csvPath )
          + " already exists. Do you want to overwrite it?'",
          QMessageBox::Yes | QMessageBox::No );
          if ( overwriteCsv == QMessageBox::No )
          {
            return;
          }
          if ( vishnucommon::Files::remove( csvPath ) )
          {
            vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
              "Can't remove existing CSV file (" + csvPath + ").", false );
            return;
          }
      }
      if ( vishnucommon::Files::exist( jsonPath ) )
      {
        QMessageBox::StandardButton overwriteJson = QMessageBox::warning( this,
          "JSON File exists", QString::fromStdString( jsonPath )
          + " already exists. Do you want to overwrite it?'",
          QMessageBox::Yes | QMessageBox::No );
          if ( overwriteJson == QMessageBox::No )
          {
            return;
          }
          if ( vishnucommon::Files::remove( jsonPath ) )
          {
            vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
              "Can't remove existing JSON file (" + jsonPath + ").", false );
            return;
          }
      }
      if ( vishnucommon::Files::exist( xmlPath ) )
      {
        QMessageBox::StandardButton overwriteXml = QMessageBox::warning( this,
          "XML File exists", QString::fromStdString( xmlPath )
          + " already exists. Do you want to overwrite it?'",
          QMessageBox::Yes | QMessageBox::No );
          if ( overwriteXml == QMessageBox::No )
          {
            return;
          }
          if ( vishnucommon::Files::remove( xmlPath ) )
          {
            vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
              "Can't remove existing XML file (" + xmlPath + ").", false );
            return;
          }
      }
    }

    vishnucommon::DataSetsPtr resultDataSets =
      _propertiesTableWidget->getDataSets( );
    vishnucommon::DataSetPtr resultDataSet =
      resultDataSets->getDataSets( ).at( 0 );
    vishnucommon::PropertyGroupsPtr propertyGroups =
      resultDataSets->getPropertyGroups( );

    //Create CSV
    if ( !createCSV( csvPath, propertyGroups ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Can't create CSV file.", false );
      return;
    }

    //Create JSON
    resultDataSets->getDataSets( ).at( 0 )->setPath( csvPath );
    if ( !createJSON( jsonPath, resultDataSets ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Can't create JSON file.", false );
      return;
    }

    //Create XML
    if ( !createXML( path, csvPath, xmlPath, resultDataSet, propertyGroups ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Can't create XML file.", false );
      return;
    }

    //Create geometric data
    if ( !createGeometricData( path ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Can't create geometric data.", false );
      return;
    }

    close( );
    setResult( QDialog::Accepted );
  }

  void DataSetWindow::slotCancelButton()
  {
    close( );
    setResult( QDialog::Rejected );
  }

   bool DataSetWindow::createCSV( const std::string& csvPath,
    const vishnucommon::PropertyGroupsPtr& propertyGroups )
  {
    bool result = false;

    //Get headers (ordered, first pk headers, then non pk headers)
    std::vector< std::string > selectedHeaders = propertyGroups->getHeaders( );
    size_t selectedHeadersSize = selectedHeaders.size( );

    //Write headers to csv
    std::string joinedHeaders = vishnucommon::Strings::join(
      selectedHeaders, std::string( "," ) );
    result = vishnucommon::Files::writeLine( csvPath, joinedHeaders );

    //Loop over files
    vishnucommon::DataSetsPtr dataSets = _dataSetListWidget->getDataSets( );
    for ( const auto& dataSet : dataSets->getDataSets( ) )
    {
      vishnucommon::Matrix csvData =
        vishnucommon::Files::readCsv( dataSet->getPath( ) );

      //Remove csv columns not used
      std::vector< std::string > oldCsvHeaders = dataSet->getPropertyNames( );
      for ( unsigned int i = 0; i < oldCsvHeaders.size( ); ++i )
      {
        if ( vishnucommon::Vectors::find( selectedHeaders, oldCsvHeaders.at( i ) )
          == -1 )
        {
          unsigned int index = static_cast< unsigned int >(
            vishnucommon::Vectors::find( csvData.at( 0 ),
            oldCsvHeaders.at( i ) ) );
          vishnucommon::Matrices::removeColumn( csvData, index );
        }
      }

      //Loop over global headers and map results
      std::map< size_t, std::string > csvHeadersMap;
      std::vector< std::string > csvHeaders = vishnucommon::Strings::split(
        vishnucommon::Strings::joinAndTrim( csvData.at( 0 ), "," ), ',');
      for ( size_t i = 0; i < selectedHeadersSize; ++i )
      {
        std::string header = selectedHeaders.at( i );
        if ( vishnucommon::Vectors::find( csvHeaders, header ) != -1 )
        {
          csvHeadersMap[ i ] = selectedHeaders.at( i );
        }
      }

      //Loop over all rows except headers
      for ( size_t row = 1; row < csvData.size( ); ++row )
      {
          std::string line;
          std::vector< std::string > csvRow = csvData.at( row );
          //Loop over headers size instead of csv columns
          for ( size_t col = 0; col < selectedHeadersSize; ++col )
          {
            if ( csvHeadersMap.find( col ) != csvHeadersMap.end( ) )
            {
              std::string field = vishnucommon::Strings::trim( csvRow.at( col ) );
              line += ( line.empty( ) ) ? field
                : std::string( "," ) + field;
            }
            else
            {
              line += ( line.empty( ) ) ? MISSING_DATA_FIELD
                : std::string( "," ) + MISSING_DATA_FIELD;
            }
          }
          result = vishnucommon::Files::writeLine( csvPath, line,
            true );
      }
    }
    return result;
  }

  bool DataSetWindow::createXML( const std::string& path,
    const std::string& csvPath, const std::string& xmlPath,
    const vishnucommon::DataSetPtr& resultDataSet,
    const vishnucommon::PropertyGroupsPtr& propertyGroups )
  {
    bool result = false;

    vishnucommon::Properties properties = resultDataSet->getProperties( );

    //Features
    std::string geometryColumn;

    vishnucommon::FeaturesVector featuresVector;
    for ( unsigned int i = 0; i < properties.size( ); ++i )
    {
      vishnucommon::PropertyPtr property = properties.at( i );
      std::string name = property->getName( );
      vishnucommon::DataCategory dataCategory = property->getDataCategory( );

      //if PK do not include as pyramidal feature
      if ( vishnucommon::Vectors::find( propertyGroups->getUsedPrimaryKeys( ),
        name ) == -1 )
      {
        //if axes (geometric points xyz) do not include as pyramidal feature
        if ( vishnucommon::Vectors::find( propertyGroups->getAxes( ), name )
          == -1 )
        {
          featuresVector.emplace_back( vishnucommon::FeaturePtr(
            new vishnucommon::Feature(
            name,
            name,
            "mV",
            dataCategory,
            property->getDataStructureType( ) ) ) );
        }
      }

      if ( dataCategory == vishnucommon::DataCategory::Geometric )
      {
        geometryColumn = name;
      }
    }

    vishnucommon::FeaturesPtr features( new vishnucommon::Features(
      propertyGroups->getUsedPrimaryKeys( ), propertyGroups->getAxes( ),
      geometryColumn, featuresVector ) );

    //Set
    vishnucommon::Sets sets;
    sets.emplace_back( vishnucommon::SetPtr( new vishnucommon::Set( csvPath,
      path + std::string( "/" ) + GEOMETRY_DATA_FOLDER ) ) );

    //Data
    vishnucommon::DataPtr dataPtr( new vishnucommon::Data( "customDataSet", "",
      features, sets ) );

    //Colors
    vishnucommon::Vec3Ptr additionalMeshesColor(
      new vishnucommon::Vec3( "60", "60", "60" ) );
    vishnucommon::Vec3Ptr backgroundColor(
      new vishnucommon::Vec3( "0", "0", "0" ) );
    vishnucommon::ColorsPtr colors(
      new vishnucommon::Colors( additionalMeshesColor, backgroundColor ) );

    //Camera
    vishnucommon::Vec3Ptr eye(
      new vishnucommon::Vec3( "577.183", "1106.67", "290.011" ) );
    vishnucommon::Vec3Ptr center(
      new vishnucommon::Vec3( "479.859", "-26.1715", "670.239" ) );
    vishnucommon::Vec3Ptr up(
      new vishnucommon::Vec3( "0.0183558", "-0.319558", "-0.947389" ) );
    vishnucommon::CameraPtr camera( new vishnucommon::Camera( eye, center, up ) );

    //Configuration
    vishnucommon::ConfigurationPtr configuration( new vishnucommon::Configuration(
      "PyramidalExplorer", "0.2.0", "data", dataPtr, colors, camera ) );

    //PyramidalXML
    std::string dtd =
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">";
    vishnucommon::PyramidalXMLPtr pyramidalXML(
      new vishnucommon::PyramidalXML( dtd, configuration ) );

    result = vishnucommon::XML::serialize( xmlPath, pyramidalXML );

    return result;
  }

  bool DataSetWindow::createJSON( const std::string& jsonPath,
    vishnucommon::DataSetsPtr& dataSets )
  {
    return vishnucommon::JSON::serialize( jsonPath, dataSets );
  }

  bool DataSetWindow::createGeometricData( const std::string& path )
  {
    //Create geometric data folder
    std::string geometryFolder = path + std::string( "/" )
      + GEOMETRY_DATA_FOLDER;
    QDir qGeometryFolder( QString::fromStdString( geometryFolder ) );
    if ( !qGeometryFolder.mkpath( QString::fromStdString( geometryFolder ) ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Can't create " + geometryFolder + " folder.", false );
      return false;
    }

    //Copy geometry files
    QString qPath = QString::fromStdString( path );
    QDir qDir( qPath );
    std::string sourceGeometryFolder = qDir.absolutePath( ).toStdString( )
      + std::string( "/" ) + GEOMETRY_DATA_FOLDER;
    QDir qSourceGeometryFolder(
      QString::fromStdString( sourceGeometryFolder ) );

    for( const QFileInfo& info : qSourceGeometryFolder.entryInfoList(
      /*QDir::Dirs |*/ QDir::Files | QDir::NoDotAndDotDot ) ) {

        QString srcFilePath =
          qSourceGeometryFolder.absoluteFilePath( info.fileName( ) );
        QString dstFilePath =
          qGeometryFolder.absoluteFilePath( info.fileName( ) );

        /*if ( info.isDir() )
        {
          //Recursive function to copy nested dirs...
        } else */
        if ( info.isFile( ) )
        {
          if ( !QFile::copy( srcFilePath, dstFilePath ) )
          {
            vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
              "Can't copy " + info.fileName( ).toStdString( )
              + " file.", false );
            return false;
          }
        }
    }
    return true;
  }

}
