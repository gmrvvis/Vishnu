#include "DataSetWindow.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>

#include <map>

#include "Definitions.hpp"
#include "UserDataSets.h"

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

  void DataSetWindow::slotAddFiles(
    const std::vector< std::string >& dropped )
  {
    DataSetWidgets dataSetWidgets = _dataSetListWidget->addDataSets( dropped );

    for ( const auto& dataSetWidget : dataSetWidgets )
    {
      QObject::connect( dataSetWidget, SIGNAL( removeSelected( ) ), this,
        SLOT( slotRemoveDataSet( ) ) );

      _propertiesTableWidget->addProperties( dataSetWidget->getHeaders( ) );

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
        sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
          "Can't create " + path + " folder.", false );
        return;
      }
    }
    else
    {
      if ( sp1common::Files::exist( csvPath ) )
      {
        QMessageBox::StandardButton overwriteCsv = QMessageBox::warning( this,
          "CSV File exists", QString::fromStdString( csvPath )
          + " already exists. Do you want to overwrite it?'",
          QMessageBox::Yes | QMessageBox::No );
          if ( overwriteCsv == QMessageBox::No )
          {
            return;
          }
          if ( sp1common::Files::remove( csvPath ) )
          {
            sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
              "Can't remove existing CSV file (" + csvPath + ").", false );
            return;
          }
      }
      if ( sp1common::Files::exist( xmlPath ) )
      {
        QMessageBox::StandardButton overwriteXml = QMessageBox::warning( this,
          "XML File exists", QString::fromStdString( xmlPath )
          + " already exists. Do you want to overwrite it?'",
          QMessageBox::Yes | QMessageBox::No );
          if ( overwriteXml == QMessageBox::No )
          {
            return;
          }
          if ( sp1common::Files::remove( xmlPath ) )
          {
            sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
              "Can't remove existing XML file (" + xmlPath + ").", false );
            return;
          }
      }
    }

    sp1common::DataSetsPtr resultDataSets =
      _propertiesTableWidget->getDataSets( );
    sp1common::DataSetPtr resultDataSet =
      resultDataSets->getDataSets( ).at( 0 );
    sp1common::PropertyGroupsPtr propertyGroups =
      resultDataSets->getPropertyGroups( );

    //Create CSV
    if ( !createCSV( csvPath, propertyGroups ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "Can't create CSV file.", false );
      return;
    }

    //Create XML
    if ( !createXML( path, csvPath, xmlPath, resultDataSet, propertyGroups ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "Can't create XML file.", false );
      return;
    }

    //Create JSON
    resultDataSets->getDataSets( ).at( 0 )->setPath( csvPath );
    if ( !createJSON( jsonPath, resultDataSets ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "Can't create JSON file.", false );
      return;
    }

    //Create JSON DataSet (userdata)
    std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );
    std::string userDataSetsFilename = userDataFolder + USER_DATASETS_FILENAME;
    UserDataSetsPtr userDataSets( new UserDataSets( ) );
    if ( sp1common::Files::exist( userDataSetsFilename ) )
    {
      userDataSets =
        sp1common::JSON::deserialize< UserDataSets >( userDataSetsFilename);
    }
    UserDataSetPtr userDataSet = _pathsWidget->getUserDataSet( );
    userDataSets->addUserDataSet( userDataSet );
    QDir qUserDataFolder( QString::fromStdString( userDataFolder ) );
    if ( !qUserDataFolder.exists( ) )
    {
      if ( !qUserDataFolder.mkpath( QString::fromStdString( userDataFolder ) ) )
      {
        sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
          "Can't create " + userDataFolder + " folder.", false );
        return;
      }
    }
    if ( !sp1common::JSON::serialize( userDataSetsFilename, userDataSets ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "Can't create user datasets file.", false );
      return;
    }

    close( );
  }

  void DataSetWindow::slotCancelButton()
  {
    close( );
  }

  bool DataSetWindow::createCSV( const std::string& csvPath,
    const sp1common::PropertyGroupsPtr& propertyGroups )
  {
    bool result = false;

    //Get headers (ordered, first pk headers, then non pk headers)
    std::vector< std::string > headers = propertyGroups->getHeaders( );
    size_t headersSize = headers.size( );

    //Write headers to csv
    std::string joinedHeaders = sp1common::Strings::join(
      headers, std::string( "," ) );
    result = sp1common::Files::writeLine( csvPath, joinedHeaders );

    //Loop over files
    for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
    {
      sp1common::Matrix csvData =
        sp1common::Files::readCsv( dataSet.second->getPath( ) );

      //Remove csv columns not used
      std::vector< std::string > oldCsvHeaders = dataSet.second->getHeaders( );
      for ( unsigned int i = 0; i < oldCsvHeaders.size( ); ++i )
      {
        if ( sp1common::Vectors::find( headers, oldCsvHeaders.at( i ) ) == -1 )
        {
          sp1common::Matrices::removeColumn( csvData, i );
        }
      }

      //Loop over global headers and map results
      std::map< size_t, std::string > csvHeadersMap;
      std::vector< std::string > csvHeaders = sp1common::Strings::split(
        sp1common::Strings::joinAndTrim( csvData.at( 0 ), "," ), ',');
      for ( size_t i = 0; i < headersSize; ++i )
      {
        std::string header = headers.at( i );
        if ( sp1common::Vectors::find( csvHeaders, header ) != -1 )
        {
          csvHeadersMap[ i ] = headers.at( i );
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
            if ( csvHeadersMap.find( col ) != csvHeadersMap.end( ) )
            {
              std::string field = sp1common::Strings::trim( csvRow.at( col ) );
              line += ( line.empty( ) ) ? field
                : std::string( "," ) + field;
            }
            else
            {
              line += ( line.empty( ) ) ? MISSING_DATA_FIELD
                : std::string( "," ) + MISSING_DATA_FIELD;
            }
          }
          result = sp1common::Files::writeLine( csvPath, line,
            true );
      }
    }
    return result;
  }

  bool DataSetWindow::createXML( const std::string& path,
    const std::string& csvPath, const std::string& xmlPath,
    const sp1common::DataSetPtr& resultDataSet,
    const sp1common::PropertyGroupsPtr& propertyGroups )
  {
    bool result = false;

    sp1common::Properties properties = resultDataSet->getProperties( );

    //Features
    std::string joinedPrimaryKey = sp1common::Strings::joinAndTrim(
      propertyGroups->getPrimaryKeys( ), std::string( "," ) );

    std::vector< std::string > positionsXYZColumn = propertyGroups->getAxes( );

    sp1common::FeaturesVector featuresVector;
    for ( unsigned int i = 0; i < properties.size( ); ++i )
    {
      sp1common::PropertyPtr property = properties.at( i );
      featuresVector.emplace_back( sp1common::FeaturePtr(
        new sp1common::Feature(
        property->getName( ),
        property->getName( ),
        "mV",
        property->getDataType( ),
        property->getDataStructureType( ) ) ) );
    }

    sp1common::FeaturesPtr features( new sp1common::Features( joinedPrimaryKey,
      positionsXYZColumn, featuresVector));

    //Set
    sp1common::Sets sets;
    sets.emplace_back( sp1common::SetPtr( new sp1common::Set( csvPath,
      path + std::string( "/" ) + GEOMETRIC_DATA_FOLDER ) ) );

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
    std::string dtd =
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">";
    sp1common::PyramidalXMLPtr pyramidalXML(
      new sp1common::PyramidalXML( dtd, configuration ) );

    result = sp1common::XML::serialize( xmlPath, pyramidalXML );

    return result;
  }

  bool DataSetWindow::createJSON( const std::string& jsonPath,
    sp1common::DataSetsPtr& dataSets )
  {
    return sp1common::JSON::serialize( jsonPath, dataSets );
  }

  /*bool DataSetWindow::generateDataFiles( QDir dir )
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
    std::string csvOutFile = dir.absolutePath( ).toStdString( )
      + std::string( "/data.csv" );

    //Get headers (ordered, first pk headers, then non pk headers)
    std::vector< std::string > headers = propertyGroups->getHeaders( );
    size_t headersSize = headers.size( );

    //Write headers to csv
    std::string joinedHeaders = sp1common::Strings::join(
      headers, std::string( "," ) );
    result = sp1common::Files::writeCsv( csvOutFile, joinedHeaders );

    //Loop over datasets
    for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
    {
      sp1common::Matrix csvData =
        sp1common::Files::readCsv( dataSet.second->getPath( ) );

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
          result = sp1common::Files::writeCsv( csvOutFile, line,
            true );
      }
    }

    sp1common::Properties properties = resultDataSet->getProperties( );

    //Create XML file
    std::string xmlOutFile = dir.absolutePath( ).toStdString( ) + "/data.xml";

    //Features
    std::string joinedPrimaryKey = sp1common::Strings::joinAndTrim(
      propertyGroups->getPrimaryKeys( ), std::string( "," ) );

    std::vector< std::string > positionsXYZColumn = propertyGroups->getAxes( );

    sp1common::FeaturesVector featuresVector;
    for ( unsigned int i = 0; i < properties.size( ); ++i )
    {
      sp1common::PropertyPtr property = properties.at( i );
      featuresVector.emplace_back( sp1common::FeaturePtr(
        new sp1common::Feature(
        property->getName( ),
        property->getName( ),
        "mV",
        property->getDataType( ),
        property->getDataStructureType( ) ) ) );
    }

    sp1common::FeaturesPtr features( new sp1common::Features( joinedPrimaryKey,
      positionsXYZColumn, featuresVector));

    //Set
    sp1common::Sets sets;
    sets.emplace_back( sp1common::SetPtr( new sp1common::Set( csvOutFile,
      dir.absolutePath( ).toStdString( ) + std::string( "/" )
      + GEOMETRIC_DATA_FOLDER ) ) );

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
    std::string dtd =
      "<!DOCTYPE configuration SYSTEM \"http://gmrv.es/pyramidalexplorer/PyramidalExplorerData-0.2.0.dtd\">";
    sp1common::PyramidalXMLPtr pyramidalXML(
      new sp1common::PyramidalXML( dtd, configuration ) );

    result = sp1common::XML::serialize( xmlOutFile, pyramidalXML );

    return result;
  }*/

}
