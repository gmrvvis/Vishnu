#include "DataSetWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

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
    QAction* actionAddDataSet = new QAction( );
    actionAddDataSet->setIcon( QIcon( ":/icons/addDataSet.png" ) );
    actionAddDataSet->setText( "AddDataSet" );
    actionAddDataSet->setToolTip( "Add Data Set" );
    actionAddDataSet->setShortcut( QKeySequence( "Ctrl+O" ) );
    QObject::connect( actionAddDataSet, SIGNAL( triggered() ), this,
      SLOT( addDataSetItems( ) ) );
    _toolBar->addAction( actionAddDataSet );

    //Widget Layout
    QVBoxLayout* widgetVBoxLayout = new QVBoxLayout();
    setLayout( widgetVBoxLayout );

    //DataSetListWidget
    _dataSetListWidget.reset( new DataSetListWidget( ) );

    qRegisterMetaType< std::vector< std::string > >("StringVector");

    QObject::connect( _dataSetListWidget.get( ),
      SIGNAL( addDataSetsEvent( const std::vector< std::string >& ) ), this,
      SLOT( addDataSetItems( const std::vector< std::string >& ) ) );

    //PropertiesTableWidget
    _propertiesTableWidget.reset( new PropertiesTableWidget( ) );

    //Buttons
    QHBoxLayout* buttonsHBoxLayout = new QHBoxLayout();
    _cancelButton = new QPushButton("Cancel", this);
    QObject::connect( _cancelButton, SIGNAL( clicked( ) ), this,
      SLOT( slotCancelButton( ) ) );
    _createButton = new QPushButton("Create", this);
    QObject::connect( _createButton, SIGNAL( clicked( ) ), this,
      SLOT( slotCreateButton( ) ) );
    buttonsHBoxLayout->addWidget( _cancelButton, 0 );
    buttonsHBoxLayout->addWidget( _createButton, 1 );

    //Fill layout with toolbar, datasets, properties and buttons
    widgetVBoxLayout->addWidget( _toolBar, 0 );
    widgetVBoxLayout->addWidget( _dataSetListWidget.get( ), 1 );
    widgetVBoxLayout->addWidget( _propertiesTableWidget.get( ), 2 );
    widgetVBoxLayout->addLayout( buttonsHBoxLayout );
  }

  DataSetWindow::~DataSetWindow()
  {

  }

  void DataSetWindow::addDataSetItems(
    const std::vector< std::string >& dropped )
  {
    std::cout << "addDataSetItems: " << dropped.size( ) << std::endl;
    DataSetWidgets dataSetWidgets = _dataSetListWidget->addDataSets( dropped );

    for ( const auto& dataSetWidget : dataSetWidgets )
    {
      QObject::connect( dataSetWidget, SIGNAL( removeSelected( ) ), this,
        SLOT( removeDataSetItem( ) ) );

      _propertiesTableWidget->addProperties( dataSetWidget->getHeaders( ) );

      _propertiesTableWidget->checkPrimaryKeys(
        _dataSetListWidget->getCommonProperties( ) );
    }
  }

  void DataSetWindow::removeDataSetItem( )
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
    //Destroy && create temporary folder
    QString vishnuTempPath = QDir::tempPath( ) + "/" + "vishnu";
    QDir vishnuTempDir( vishnuTempPath );
    if ( vishnuTempDir.exists( ) )
    {
      vishnuTempDir.removeRecursively( );
    }

    if ( !vishnuTempDir.mkpath( vishnuTempPath ) )
    {
        sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
          "Can't create " + vishnuTempPath.toStdString( ) + " folder.",
          false );
        return;
    }

    if ( !generateDataFiles( vishnuTempDir ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
        "Can't create data files.", false );
      return;
    }

    close( );
  }

  void DataSetWindow::slotCancelButton()
  {
    close( );
  }

  bool DataSetWindow::generateDataFiles( QDir dir )
  {
    sp1common::Debug::consoleMessage( "Generating app data files in folder: "
      + dir.absolutePath( ).toStdString( ) );

    bool result = false;
    bool pkInUse = false;
    std::vector< std::string > primaryKeys;
    std::vector< bool > use;
    std::vector< std::string > propertyNames;
    std::vector< std::string > propertyDataTypes;
    std::string xAxis;
    std::string yAxis;
    std::string zAxis;
    std::string xyzAxis;
    //bool geometricDataField = false;

    for ( auto const& property : _propertiesTableWidget->getProperties( ) )
    {
      /*if ( sp1common::Strings::lower( property->getName( ) )
        == GEOMETRIC_DATA_FIELD )
      {
        geometricDataField = true;
      }*/

      use.push_back( property->getUse( ) );
      if ( property->getUse( ) )
      {
        if ( property->getPrimaryKey( ) )
        {
          for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
          {
            std::vector< std::string > dataSetHeaders =
              dataSet.second->getHeaders( );
            if ( std::find( dataSetHeaders.begin( ), dataSetHeaders.end( ),
              property->getName( ) ) == dataSetHeaders.end( ) )
            {
              //Property is primary but DataSet doesn't contains the property
              return false;
            }
            else
            {
              //There are at least one primary key in use
              pkInUse = true;
            }
          }
          primaryKeys.push_back( property->getName( ) );
        }
        else
        {
          propertyNames.push_back( property->getName( ) );
          propertyDataTypes.push_back( sp1common::toString(
            property->getDataType( ) ) );
        }
      }
      sp1common::AxisType axisType = property->getAxisType( );
      switch( axisType )
      {
        case sp1common::AxisType::None:
          //Do nothing (avoid warning)
          break;
        case sp1common::AxisType::X:
          xAxis = property->getName( );
          break;
        case sp1common::AxisType::Y:
          yAxis = property->getName( );
          break;
        case sp1common::AxisType::Z:
          zAxis = property->getName( );
          break;
        case sp1common::AxisType::XYZ:
          xyzAxis = property->getName( );
          break;
      }
    }

    if ( !pkInUse )
    {
      const char *message = "There must be at least one primary key in "
        "properties table.";

      QMessageBox::question( this, APPLICATION_NAME, message, QMessageBox::Ok );
      sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
        message, false );

      return false;
    }

    if ( xyzAxis.empty( ) && ( xAxis.empty( ) || yAxis.empty( )
      || zAxis.empty( ) ) )
    {
      const char *message = "There must be at least one property selected as "
        "XYZ axis or three properties as X, Y and Z axis in properties table.";

      QMessageBox::question( this, APPLICATION_NAME, message, QMessageBox::Ok );
      sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
        message, false );

      return false;
    }

    /*std::string primaryKeyName = sp1common::Strings::joinAndTrim( primaryKeys,
      "-" );*/
    std::string primaryKeyName = "Key";

    //Create CSV with data
    std::string csvOutFile = dir.absolutePath( ).toStdString( ) + "/data.csv";

    std::string headers = sp1common::Strings::join( propertyNames, "," );
    //headers = primaryKeysJoined + "," + headers;
    headers = primaryKeyName + "," + headers + "\n";

    result = sp1common::Files::writeCsv( csvOutFile, headers );
    if ( !result )
    {
      const char *message = "Can't create CSV file.";

      QMessageBox::question( this, APPLICATION_NAME, message, QMessageBox::Ok );
      sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
        message, false );

      return false;
    }

    for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
    {
      sp1common::Matrix csvData = sp1common::Files::readCsv(
        dataSet.second->getPath( ) );

      std::vector< unsigned int > primaryKeyIndices;
      std::vector< std::string > dataSetHeaders =
        dataSet.second->getHeaders( );
      for ( unsigned int i = 0; i < dataSetHeaders.size( ); ++i )
      {
        if ( std::find( primaryKeys.begin( ), primaryKeys.end( ),
          dataSetHeaders.at( i ) ) != primaryKeys.end( ) )
        {
          primaryKeyIndices.push_back( i );
        }
      }

      //Starts at 1 (omit headers)
      for ( unsigned int row = 1; row < csvData.size( ); ++row )
      {
        std::string pkLine;
        std::string line;
        std::vector< std::string > csvRow = csvData.at( row );

        for ( unsigned int col = 0; col < csvRow.size(); ++col )
        {
          if ( std::find( primaryKeyIndices.begin( ),
            primaryKeyIndices.end( ), col ) != primaryKeyIndices.end( ) )
          {
            pkLine += ( pkLine.empty( ) )
              ? sp1common::Strings::trim( csvRow.at( col ) )
              : "-" + sp1common::Strings::trim( csvRow.at( col ) );
          }
          else
          {
            if ( use[ col ] )
            {
              line += ( line.empty( ) ) ? csvRow.at( col )
                : "," + csvRow.at( col );
            }
          }
        }

        // /*dataset name + */ primary key, rest of fields
        line = /*dataSet.first + */ pkLine + "," + line + "\n";
        result = sp1common::Files::writeCsv( csvOutFile, line, true );
        if ( !result )
        {
          sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
            "Can't create CSV file.", false );
          return false;
        }
      }
    }

    //Create XML file
    std::string xmlOutFile = dir.absolutePath( ).toStdString( ) + "/data.xml";

    result = sp1common::Files::writeXmlStartDocument( xmlOutFile );

    result = sp1common::Files::writeXmlDTD( xmlOutFile,
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">"
    );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "configuration" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "app", "PyramidalExplorer" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "version", "0.2.0" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "label", "data" );
        result = sp1common::Files::writeXmlCloseElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "data" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "objectsLabel", "customDataSet" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "additionalMeshesLabel", "" );
    result = sp1common::Files::writeXmlCloseElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "features" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "idColumn", primaryKeyName );
    if ( !xyzAxis.empty( ) )
    {
      result = sp1common::Files::writeXmlAttribute( xmlOutFile,
        "positionXYZColumn", xyzAxis );
    }
    else
    {
      result = sp1common::Files::writeXmlAttribute( xmlOutFile,
        "positionXColumn", xAxis );
      result = sp1common::Files::writeXmlAttribute( xmlOutFile,
        "positionYColumn", yAxis );
      result = sp1common::Files::writeXmlAttribute( xmlOutFile,
        "positionZColumn", zAxis );
    }
    result = sp1common::Files::writeXmlCloseElement( xmlOutFile );

    for ( unsigned int i = 0; i < propertyNames.size( ); ++i )
    {
        result = sp1common::Files::writeXmlStartElement( xmlOutFile,
          "feature" );
        result = sp1common::Files::writeXmlAttribute( xmlOutFile,
          "sourceColumn", propertyNames.at( i ) );
        result = sp1common::Files::writeXmlAttribute( xmlOutFile,
          "applicationLabel", propertyNames.at( i ) );
        result = sp1common::Files::writeXmlAttribute( xmlOutFile,
          "unitsLabel", "mV" );
        result = sp1common::Files::writeXmlAttribute( xmlOutFile,
          "dataType", propertyDataTypes.at( i ) );
        result = sp1common::Files::writeXmlAttribute( xmlOutFile,
          "dataStructureType", "none" );
        result = sp1common::Files::writeXmlEndElement( xmlOutFile );
    }
    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "features" );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "set" );
    result = sp1common::Files::writeXmlCloseElement( xmlOutFile );
    result = sp1common::Files::writeXmlTextElement( xmlOutFile,
      "objectsFeaturesCSV", csvOutFile );
    result = sp1common::Files::writeXmlTextElement( xmlOutFile,
      "objectsMeshesPath", dir.absolutePath( ).toStdString( )
      + "/geometricData/" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "set" );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "colors" );
    result = sp1common::Files::writeXmlCloseElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "additionalMeshesColor" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "r", "60" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "g", "60" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "b", "60" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "backgroundColor" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "r", "0" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "g", "0" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "b", "0" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile );

    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "colors" );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "camera" );
    result = sp1common::Files::writeXmlCloseElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "eye" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "x", "577.183" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "y", "1106.67" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "z", "290.011" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "center" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "x", "479.859" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "y", "-26.1715" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "z", "670.239" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile );

    result = sp1common::Files::writeXmlStartElement( xmlOutFile,
      "up" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "x", "0.0183558" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "y", "-0.319558" );
    result = sp1common::Files::writeXmlAttribute( xmlOutFile,
      "z", "-0.947389" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile );

    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "camera" );

    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "data" );
    result = sp1common::Files::writeXmlEndElement( xmlOutFile, "configuration" );
    result = sp1common::Files::writeXmlEndDocument( xmlOutFile );

    return result;
  }

}
