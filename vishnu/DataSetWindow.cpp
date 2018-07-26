#include "DataSetWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

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
    sp1common::DataSetsPtr resultDataSets =
      _propertiesTableWidget->getDataSets( );
    sp1common::DataSetPtr resultDataSet =
      resultDataSets->getDataSets( ).at( 0 );
    sp1common::PropertyGroupsPtr propertyGroups =
      resultDataSets->getPropertyGroups( );
    std::string csvOutFile = dir.absolutePath( ).toStdString( ) + "/data.csv";

    std::vector< std::string > headers = propertyGroups->getUse( );
    size_t headersSize = headers.size( );

    //Write headers to csv
    std::string joinedHeaders = sp1common::Strings::join( headers, "," );
    result = sp1common::Files::writeCsv( csvOutFile, joinedHeaders );

    //Loop over datasets
    for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
    {
      sp1common::Matrix csvData = sp1common::Files::readCsv(
        dataSet.second->getPath( ) );

      //Remove csv columns not used
      std::vector< std::string > oldCsvHeaders = dataSet.second->getHeaders( );
      for ( unsigned int i = 0; i < oldCsvHeaders.size( ); ++i )
      {
        if ( !sp1common::Vectors::find( headers, oldCsvHeaders.at( i ) ) )
        {
          sp1common::Matrices::removeColumn( csvData, i );
        }
      }

      //Loop over global headers and map results
      std::map< int, std::string > csvHeaders;
      for ( size_t i = 0; i < headersSize; ++i )
      {
        if ( sp1common::Vectors::find( csvData.at( 0 ), headers.at( i ) ) )
        {
          csvHeaders[ static_cast< int >( i ) ] = headers.at( i );
        }
      }

      //Loop over all rows except headers
      for ( size_t row = 1; row < csvData.size( ); ++row )
      {
          std::string line;
          std::vector< std::string > csvRow = csvData.at( row );
          //Loop over headers size instead of csv columns
          for ( size_t col = 0; col < headersSize; ++col )
          {
            if ( csvHeaders.find( static_cast< int >( col ) )
              != csvHeaders.end( ) )
            {
              line += ( line.empty( ) ) ? csvRow.at( col )
                : std::string( "," ) + csvRow.at( col );
            }
            else
            {
              line += ( line.empty( ) ) ? MISSING_DATA_FIELD
                : std::string( "," ) + MISSING_DATA_FIELD;
            }
          }
          result = sp1common::Files::writeCsv( csvOutFile, line, true );
      }
    }


    sp1common::Properties properties = resultDataSet->getProperties( );

    //Create XML file
    std::string xmlOutFile = dir.absolutePath( ).toStdString( ) + "/data.xml";

    //Features
    sp1common::FeaturesVector featuresVector;
    for ( unsigned int i = 0; i < properties.size( ); ++i )
    {
      sp1common::PropertyPtr property = properties.at( i );
      featuresVector.emplace_back( sp1common::FeaturePtr(
        new sp1common::Feature(
        QString::fromStdString( property->getName( ) ),
        QString::fromStdString( property->getName( ) ),
        "mV",
        property->getDataType( ),
        property->getDataStructureType( ) ) ) );
    }
    QStringList positionsXYZColumn;
    if ( propertyGroups->getAxes( ).size( ) == 1 )
    {
       positionsXYZColumn
         << QString::fromStdString( propertyGroups->getAxes( ).at( 0 ) );
    }
    else
    {
      positionsXYZColumn
        << QString::fromStdString( propertyGroups->getAxes( ).at( 0 ) )
        << QString::fromStdString( propertyGroups->getAxes( ).at( 1 ) )
        << QString::fromStdString( propertyGroups->getAxes( ).at( 2 ) );
    }

    std::string joinedPrimaryKey = sp1common::Strings::joinAndTrim(
      propertyGroups->getPrimaryKeys( ), "," );
    sp1common::FeaturesPtr features( new sp1common::Features(
      QString::fromStdString( joinedPrimaryKey ), positionsXYZColumn,
      featuresVector));

    //Set
    sp1common::Sets sets;
    sets.emplace_back( sp1common::SetPtr( new sp1common::Set(
      QString::fromStdString( csvOutFile ),
      dir.absolutePath( ) + "/" + GEOMETRIC_DATA_FOLDER ) ) );

    //Data
    sp1common::DataPtr dataPtr( new sp1common::Data( "customDataSet", "",
      features, sets ) );

    //Colors
    sp1common::XYZPtr additionalMeshesColor(
      new sp1common::XYZ( "60", "60", "60" ) );
    sp1common::XYZPtr backgroundColor(
      new sp1common::XYZ( "0", "0", "0" ) );
    sp1common::ColorsPtr colors(
      new sp1common::Colors( additionalMeshesColor, backgroundColor ) );

    //Camera
    sp1common::XYZPtr eye(
      new sp1common::XYZ( "577.183", "1106.67", "290.011" ) );
    sp1common::XYZPtr center(
      new sp1common::XYZ( "479.859", "-26.1715", "670.239" ) );
    sp1common::XYZPtr up(
      new sp1common::XYZ( "0.0183558", "-0.319558", "-0.947389" ) );
    sp1common::CameraPtr camera( new sp1common::Camera( eye, center, up ) );

    //Configuration
    sp1common::ConfigurationPtr configuration( new sp1common::Configuration(
      "PyramidalExplorer", "0.2.0", "data", dataPtr, colors, camera ) );

    //PyramidalXML
    QString dtd =
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">";
    sp1common::PyramidalXMLPtr pyramidalXML(
      new sp1common::PyramidalXML( dtd, configuration ) );

    result = sp1common::XML::serialize( xmlOutFile, pyramidalXML );

    return result;
  }

  /*bool DataSetWindow::generateDataFiles2( QDir dir )
  {
    sp1common::Debug::consoleMessage( "Generating app data files in folder: "
      + dir.absolutePath( ).toStdString( ) );

    bool result = false;
    bool pkInUse = false;
    std::vector< std::string > primaryKeys;
    std::vector< bool > use;
    std::vector< std::string > propertyNames;
    std::vector< sp1common::DataType > propertyDataTypes;
    std::string xAxis;
    std::string yAxis;
    std::string zAxis;
    std::string xyzAxis;
    //bool geometricDataField = false;

    for ( auto const& property : _propertiesTableWidget->getProperties( ) )
    {
      use.emplace_back( property->getUse( ) );
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
          primaryKeys.emplace_back( property->getName( ) );
        }
        else
        {
          propertyNames.emplace_back( property->getName( ) );
          propertyDataTypes.emplace_back( property->getDataType( ) );
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

    //std::string primaryKeyName = sp1common::Strings::joinAndTrim( primaryKeys,
    //"-" );
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

        // primary key, rest of fields
        line = pkLine + "," + line + "\n";
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

    //Features
    sp1common::FeaturesVector featuresVector;
    for ( unsigned int i = 0; i < propertyNames.size( ); ++i )
    {
      featuresVector.emplace_back( sp1common::FeaturePtr(
        new sp1common::Feature(
        QString::fromStdString( propertyNames.at( i ) ),
        QString::fromStdString( propertyNames.at( i ) ),
        "mV",
        propertyDataTypes.at( i ),
        sp1common::DataStructureType::None ) ) );
    }
    QStringList positionsXYZColumn;
    if ( !xyzAxis.empty( ) )
    {
       positionsXYZColumn << QString::fromStdString( xyzAxis );
    }
    else
    {
      positionsXYZColumn << QString::fromStdString( xAxis )
        << QString::fromStdString( yAxis ) << QString::fromStdString( zAxis );
    }

    sp1common::FeaturesPtr features( new sp1common::Features(
      QString::fromStdString( primaryKeyName ), positionsXYZColumn,
      featuresVector));

    //Set
    sp1common::Sets sets;
    sets.emplace_back( sp1common::SetPtr( new sp1common::Set(
      QString::fromStdString( csvOutFile ),
      dir.absolutePath( ) + "/" + GEOMETRIC_DATA_FOLDER ) ) );

    //Data
    sp1common::DataPtr dataPtr( new sp1common::Data( "customDataSet", "",
      features, sets ) );

    //Colors
    sp1common::XYZPtr additionalMeshesColor(
      new sp1common::XYZ( "60", "60", "60" ) );
    sp1common::XYZPtr backgroundColor(
      new sp1common::XYZ( "0", "0", "0" ) );
    sp1common::ColorsPtr colors(
      new sp1common::Colors( additionalMeshesColor, backgroundColor ) );

    //Camera
    sp1common::XYZPtr eye(
      new sp1common::XYZ( "577.183", "1106.67", "290.011" ) );
    sp1common::XYZPtr center(
      new sp1common::XYZ( "479.859", "-26.1715", "670.239" ) );
    sp1common::XYZPtr up(
      new sp1common::XYZ( "0.0183558", "-0.319558", "-0.947389" ) );
    sp1common::CameraPtr camera( new sp1common::Camera( eye, center, up ) );

    //Configuration
    sp1common::ConfigurationPtr configuration( new sp1common::Configuration(
      "PyramidalExplorer", "0.2.0", "data", dataPtr, colors, camera ) );

    //PyramidalXML
    QString dtd =
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">";
    sp1common::PyramidalXMLPtr pyramidalXML(
      new sp1common::PyramidalXML( dtd, configuration ) );

    result = sp1common::XML::serialize( xmlOutFile, pyramidalXML );

    return result;
  }*/

}
