#include "MainWindow.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPixmap>
#include <QComboBox>
#include <QGroupBox>

#include <iostream>
#include <stdio.h>

#include "Definitions.hpp"
#include "RegExpInputDialog.h"
#include "widgets/DataSetWidget.h"

namespace vishnu
{
  MainWindow::MainWindow( sp1common::Args args,
    QWidget *parent )
    : QMainWindow( parent )
  {
    //MenuBar
    QMenu *fileMenu = new QMenu(tr("&File"));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));
    QMenu *itemsMenu = new QMenu(tr("&DataSets"));
    addDataSetAction = itemsMenu->addAction(tr("&Add DataSet"));
    //removeAction = itemsMenu->addAction(tr("&Remove Item"));
    //QAction *ascendingAction = itemsMenu->addAction(tr("Sort in &Ascending Order"));
    //QAction *descendingAction = itemsMenu->addAction(tr("Sort in &Descending Order"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);
    //End MenuBar

    //ToolBar
    QToolBar* toolBar = new QToolBar( );
    toolBar->setMovable( false );
    QAction* actionAddDataSet = new QAction( );
    actionAddDataSet->setIcon( QIcon( ":/icons/addDataSet.png" ) );
    actionAddDataSet->setText( "AddDataSet" );
    actionAddDataSet->setToolTip( "Add Data Set" );
    actionAddDataSet->setShortcut( QKeySequence( "Ctrl+O" ) ); 
    QObject::connect( actionAddDataSet, SIGNAL( triggered() ), this,
      SLOT( addDataSetItem( ) ) );
    toolBar->addAction( actionAddDataSet );
    addToolBar(Qt::TopToolBarArea, toolBar);
    //End ToolBar

    _workingDirectory = args.get( "-wd" );
    _zeqSession = args.get( "-z" );

    qRegisterMetaType<StringVector>("StringVector");

    QObject::connect( this, SIGNAL( signalSyncGroup( const QString&,
      const QString&, const QString&, const StringVector&,
      const QColor& ) ), SLOT( syncGroup( const QString&, const QString&,
      const QString&, const StringVector&, const QColor& ) ),
      Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signalChangeGroupName( const QString&,
      const QString& ) ), SLOT( changeGroupName( const QString&,
      const QString& ) ), Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signalChangeGroupColor( const QString&,
      const QColor& ) ), SLOT( changeGroupColor( const QString&,
      const QColor& ) ), Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signalDestroyGroup( const QString& ) ),
      SLOT( removeGroup( const QString& ) ), Qt::QueuedConnection );

    //ZeqSession
    initZeqSession( );

    //Central Widget
    setCentralWidget( new QWidget( ) );

    QGridLayout* gridLayout = new QGridLayout( );
    centralWidget()->setLayout( gridLayout );

    //DataSetListWidget
    _dataSetListWidget.reset( new DataSetListWidget( ) );

    QObject::connect( addDataSetAction, SIGNAL( triggered( ) ), this,
      SLOT( addDataSetItem( ) ) );

    QObject::connect( _dataSetListWidget.get( ),
      SIGNAL( addDataSetEvent( const std::string& ) ), this,
      SLOT( addDataSetItem( const std::string& ) ) );

    //PropertiesTableWidget
    _propertiesTableWidget.reset( new PropertiesTableWidget( ) );

    //Applications
    loadClint();
    loadDCExplorer();
    loadPyramidal();

    QVBoxLayout* appsVBoxLayout = new QVBoxLayout( );
    for ( const auto& application : _applications )
    {
      appsVBoxLayout->addWidget( application.second->getPushButton( ) );

      QObject::connect( application.second->getPushButton( ),
        SIGNAL( clicked( bool ) ), this, SLOT( runApp( ) ) );
    }

    //Groups
    _zeqGroupListWidget.reset( new ZEQGroupListWidget( ) );

    //Add all widgets to grid
    QSizePolicy dsSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    dsSizePolicy.setHorizontalStretch( 255 );
    _dataSetListWidget->setSizePolicy( dsSizePolicy );

    QSizePolicy prSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    prSizePolicy.setHorizontalStretch( 1 );
    _propertiesTableWidget->setSizePolicy( prSizePolicy );

    QSizePolicy groupsSizePolicy( QSizePolicy::Preferred,
      QSizePolicy::Expanding );
    groupsSizePolicy.setHorizontalStretch( 1 );
    _zeqGroupListWidget->setSizePolicy( groupsSizePolicy );

    //DataSets && Applications
    QHBoxLayout* topHBoxLayout = new QHBoxLayout();
    topHBoxLayout->addWidget( _dataSetListWidget.get( ), 0 );
    topHBoxLayout->addLayout( appsVBoxLayout, 1 );
    gridLayout->addLayout( topHBoxLayout, 0, 0, Qt::AlignTop );

    //Properties && ZEQ Groups
    QHBoxLayout* bottomHBoxLayout = new QHBoxLayout( );
    bottomHBoxLayout->addWidget( _propertiesTableWidget.get( ), 0 );
    bottomHBoxLayout->addWidget( _zeqGroupListWidget.get( ), 1 );
    gridLayout->addLayout( bottomHBoxLayout, 1, 0, Qt::AlignTop );

    /*std::vector<std::string> ids;
    ids.push_back("test");
    syncGroup("GROUP1#!#CLINT", "GROUP1", "CLINT", ids, QColor(255, 65, 77));
    syncGroup("GROUP1#!#DCEXPLORER", "GROUP2", "DCEXPLORER", ids, QColor(3, 255, 77));
    syncGroup("GROUP2#!#DCEXPLORER", "GROUP3", "DCEXPLORER", ids, QColor(67, 67, 15));
    syncGroup("GROUP1#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(60, 128, 0));
    syncGroup("GROUP2#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(127, 79, 55));
    changeGroupColor("GROUP1#!#CLINT", QColor(0, 255, 0));
    changeGroupName("GROUP2#!#DCEXPLORER", "newName");
    syncGroup("GROUP2#!#PYRAMIDAL", "otherName", "PYRAMIDAL", ids, QColor(255, 65, 77));
    removeGroup("GROUP3#!#PYRAMIDAL");*/

    //Status bar
    statusBar()->showMessage("");
  }

  MainWindow::~MainWindow( )
  {
    //delete _ui;
    for ( const auto& appplication : _applications )
    {
      if ( appplication.second->isOpen( ) )
      {
        appplication.second->terminate( );
      }
    }
  }
  
  void MainWindow::closeEvent( QCloseEvent* e )
  {
    QMessageBox::StandardButton result = QMessageBox::question( this,
      QString( APPLICATION_NAME ), QString( "Are you sure?\n" ),
      QMessageBox::Yes | QMessageBox::No );

    if ( result != QMessageBox::Yes )
    {
      e->ignore( );
    }
    else
    {
      e->accept( );
    }
  }

  void MainWindow::addDataSetItem( const std::string& dropped )
  {
    DataSetWidget* dsw = _dataSetListWidget->addDataSet( dropped );

    if ( dsw )
    {
      QObject::connect( dsw, SIGNAL( removeSelected( ) ), this,
        SLOT( removeDataSetItem( ) ) );

      _propertiesTableWidget->addProperties( dsw->getHeaders( ) );

      _propertiesTableWidget->checkPrimaryKeys(
        _dataSetListWidget->getCommonProperties( ) );
    }
  }

  void MainWindow::removeDataSetItem( )
  {
    std::vector< std::string > propertiesToRemove =
      _dataSetListWidget->getPropertiesToRemove( );

    _propertiesTableWidget->removeProperties( propertiesToRemove );

    _dataSetListWidget->removeCurrentDataSet( );

    _propertiesTableWidget->checkPrimaryKeys(
      _dataSetListWidget->getCommonProperties( ) );
  }

  void MainWindow::syncGroup( const QString& key, const QString& name,
    const QString& owner, const StringVector& ids,
    const QColor& color )
  {
    ZEQGroupWidget* zeqGroupWidget = _zeqGroupListWidget->syncGroup(
      key.toStdString( ), name, owner, ids, color );

    if ( zeqGroupWidget )
    {
      QObject::connect( zeqGroupWidget, SIGNAL( signalChangeGroupName(
        const QString&, const QString& ) ), this, SLOT( changeGroupName(
        const QString&, const QString& ) ) );

      QObject::connect( zeqGroupWidget, SIGNAL( signalChangeGroupColor(
        const QString&, const QColor& ) ), this, SLOT( changeGroupColor(
        const QString&, const QColor& ) ) );

      QObject::connect( zeqGroupWidget, SIGNAL( signalRemoveSelectedGroup( ) ),
        this, SLOT( removeSelectedGroup( ) ) );
    }
  }

  void MainWindow::changeGroupName( const QString& key,
    const QString& name )
  {
    _zeqGroupListWidget->changeGroupName( key.toStdString( ), name );
  }

  void MainWindow::changeGroupColor( const QString& key,
    const QColor& color )
  {
    _zeqGroupListWidget->changeGroupColor( key.toStdString( ), color );
  }

  void MainWindow::removeSelectedGroup( )
  {
    //Clicked on destroy group button -> Ask && publish ZEQ event
    std::string currentKey =
      _zeqGroupListWidget->getKey( _zeqGroupListWidget->currentItem( ) );
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning( this, "Remove group",
      "Do you want to remove '" + QString::fromStdString( currentKey )
      + "'' group?", QMessageBox::Yes|QMessageBox::No );
    if ( reply == QMessageBox::Yes )
    {
      _zeqGroupListWidget->removeCurrentGroup( );
      manco::ZeqManager::instance( ).publishDestroyGroup( currentKey );
    }
  }

  void MainWindow::removeGroup( const QString& key )
  {
    //Received ZEQ event -> Don't ask and destroy
    _zeqGroupListWidget->removeGroup( key.toStdString( ) );
  }

  void MainWindow::closeApp( int exitCode, QProcess::ExitStatus exitStatus )
  {
    QProcess* qProcess = qobject_cast< QProcess* >( sender( ) );
    if ( exitStatus == QProcess::CrashExit )
    {
      std::cerr << "Error: " << qProcess->program( ).toStdString( ) <<
        " crashed!" << std::endl;
    }
    else if ( exitCode != 0 )
    {
      std::cerr << "Error: " << qProcess->program( ).toStdString( ) <<
        " failed!" << std::endl;
    }
    else
    {
      std::cout << qProcess->program( ).toStdString( ) <<
        " closed successfully." << std::endl;
    }

    //Look for running app
    for (  auto appplication : _applications )
    {
      if ( appplication.second->program( ) == qProcess->program( ) )
      {
        //app.second->setProgram( QString( ) );
        QObject::disconnect( appplication.second.get( ),
          SIGNAL( finished ( int , QProcess::ExitStatus ) ), this,
          SLOT( closeApp( int, QProcess::ExitStatus ) ) );
        appplication.second->getPushButton( )->setEnabled( true );
        break;
      }
    }
  }

  void MainWindow::checkApps( )
  {
    /*if ( !_ui->csvFilename->text( ).isEmpty( ) )
    {                
      for ( const auto& appplication : _applications )
      {

        //TODO: parse XML in order to enable buttons instead of
        //enabling all of them
        appplication.second->getPushButton( )->setEnabled( true );
      }
    }*/
  }

  bool MainWindow::generateDataFiles( QDir dir )
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
            if ( dataSet.second->getChecked( ) )
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
      AxisType axisType = property->getAxisType( );
      switch( axisType )
      {
        case AxisType::None:
          //Do nothing (avoid warning)
          break;
        case AxisType::X:
          xAxis = property->getName( );
          break;
        case AxisType::Y:
          yAxis = property->getName( );
          break;
        case AxisType::Z:
          zAxis = property->getName( );
          break;
        case AxisType::XYZ:
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
      if ( dataSet.second->getChecked( ) )
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

  void MainWindow::loadClint()
  {
    std::string displayName = "Clint";
    std::string cmd = "ClintExplorer";
    sp1common::Args args;
    if ( !_zeqSession.empty( ) )
    {
      args.set( "-z", _zeqSession );
      args.set( "-sp", 31400 );
    }
    args.set( "-ce", _workingDirectory );
    args.set( "-ch", "http://localhost" );
    args.set( "-cp", 8765 );

    /*if ( !_ui->csvFilename->text( ).isEmpty( ) )
    {
      args.set( "-f", _ui->csvFilename->text( ).toStdString( ) );
    }*/

   std::string instanceId = sp1common::Strings::generateRandom( 5 );
   args.set( "-id", instanceId );

    ApplicationPtr appplication( new Application(
      sp1common::ApplicationType::CLINT, displayName, cmd, args,
      QIcon(":/icons/logoClint.png"), _workingDirectory ) );

    std::string owner = sp1common::toString(
      sp1common::ApplicationType::CLINT ) + instanceId;

    _applications[ owner ] = appplication;
  }

  void MainWindow::loadDCExplorer()
  {
    std::string displayName = "DCExplorer";
    std::string cmd = "WeCo";
    std::string dockerContainerName = "apicolat";

    sp1common::Args args;

    if ( !_zeqSession.empty( ) )
    {
      args.set( "-z", _zeqSession );
      args.set( "-p", 12345 );
    }
    args.set( "-u", "http://localhost:8888" );
    args.set( "-n", displayName );
    args.set( "-dcn", dockerContainerName );

    /*if ( !_ui->csvFilename->text( ).isEmpty( ) )
    {
      args.set( "-f", _ui->csvFilename->text( ).toStdString( ) );
    }*/

    std::string instanceId = sp1common::Strings::generateRandom( 5 );
    args.set( "-id", instanceId );

    ApplicationPtr application( new Application(
      sp1common::ApplicationType::DCEXPLORER, displayName, cmd, args,
      QIcon(":/icons/logoDCExplorer.png"), _workingDirectory ) );

    std::string owner = sp1common::toString(
      sp1common::ApplicationType::DCEXPLORER ) + instanceId;

    _applications[ owner ] = application;
  }

  void MainWindow::loadPyramidal()
  {
    std::string displayName = "Pyramidal";
    std::string cmd = "CellExplorer";

    sp1common::Args args;
    args.set( "-z", _zeqSession );

    /*if ( !_ui->csvFilename->text( ).isEmpty( ) )
    {
      args.set( "-f", _ui->csvFilename->text( ).toStdString( ) );
    }*/

    std::string instanceId = sp1common::Strings::generateRandom( 5 );
    args.set( "-id", instanceId );

    ApplicationPtr application( new Application(
      sp1common::ApplicationType::PYRAMIDAL, displayName, cmd, args,
      QIcon(":/icons/logoPyramidal.png"), _workingDirectory ) );

    std::string owner = sp1common::toString(
      sp1common::ApplicationType::PYRAMIDAL ) + instanceId;

    _applications[ owner ] = application;
  }

  void MainWindow::runApp( )
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

    QPushButton* appButton = qobject_cast< QPushButton* >( sender( ) );
    if ( !appButton )
    {
      std::cerr << "Error: App not found!" << std::endl;
      return;
    }

    for (const auto& it : _applications )
    {
      if ( it.second->getPushButton( ) == appButton)
      {        
        sp1common::Debug::consoleMessage( "Opening "
          + it.second->getDisplayName( ) );

        it.second->getPushButton( )->setEnabled( false );
        it.second->setReadChannel( QProcess::StandardOutput );
        //it.second->setProcessChannelMode(QProcess::ForwardedChannels); //DEBUG
        it.second->waitForReadyRead( );
        it.second->readAllStandardOutput( );

        QStringList arguments;
        for(const auto& arg : it.second->getArgs( ) )
        {
          arguments << QString::fromStdString( arg.first );
          if ( !arg.second.empty( ) )
          {
            arguments << QString::fromStdString( arg.second );
          }
        }

        it.second->start(
          QString::fromStdString( it.second->getCmd( ) ),
          arguments
        );

        QByteArray out_data = it.second->readAllStandardOutput( );
        QString out_string( out_data );
        std::cout << out_string.toStdString( ).c_str( ) << std::endl;
        QObject::connect( it.second.get( ),
          SIGNAL( finished ( int , QProcess::ExitStatus ) ), this,
          SLOT( closeApp( int , QProcess::ExitStatus ) ) );
        break;
      }
    }
  }

  void MainWindow::initZeqSession( )
  {
    std::cout << "Init ZeqSession ( " << _zeqSession << " )" << std::endl;
    manco::ZeqManager::instance( ).init( _zeqSession );
    manco::ZeqManager::instance( ).setReceivedSyncGroupCallback(
      std::bind( &MainWindow::receivedSyncGroup, this,
      std::placeholders::_1 ) );
    manco::ZeqManager::instance( ).setReceivedChangeNameGroupUpdateCallback(
      std::bind( &MainWindow::receivedChangeNameGroup, this,
      std::placeholders::_1 ) );
    manco::ZeqManager::instance( ).setReceivedChangeColorUpdateCallback(
      std::bind( &MainWindow::receivedChangeColorGroup, this,
      std::placeholders::_1 ) );
    manco::ZeqManager::instance( ).setReceivedDestroyGroupCallback(
      std::bind( &MainWindow::receivedDestroyGroup, this,
      std::placeholders::_1 ) );
  }

  void MainWindow::receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o )
  {
    std::vector<unsigned int> color = o->getColorVector( );
    std::vector< std::string > vectorIds = manco::ZeqManager::split(
      o->getIdsString( ), DELIMITER );

    std::stringstream message;
    message << "Received SyncGroup ("
      << "\n\tkey: " << o->getKeyString( )
      << "\n\tname: " << o->getNameString( )
      << "\n\towner: " << o->getOwnerString( )
      << "\n\tcolor: ("
      << color[0] << ", "
      << color[1] << ", "
      << color[2] << ")"
      << "\n\tids: (... commented ...)";
    sp1common::Debug::consoleMessage( message.str( ) );

    emit signalSyncGroup(
      QString::fromStdString( o->getKeyString( ) ),
      QString::fromStdString( o->getNameString( ) ),
      QString::fromStdString( o->getOwnerString( ) ),
      vectorIds,
      QColor( static_cast<int>(color[0]), static_cast<int>(color[1]),
        static_cast<int>(color[2]) )
    );
  }

  void MainWindow::receivedChangeNameGroup(
    zeroeq::gmrv::ConstChangeNameGroupPtr o )
  {
    emit signalChangeGroupName( QString::fromStdString( o->getKeyString( ) ),
      QString::fromStdString( o->getNameString( ) ) );
  }

  void MainWindow::receivedChangeColorGroup(
    zeroeq::gmrv::ConstChangeColorGroupPtr o )
  {
    std::vector<unsigned int> color = o->getColorVector( );
    emit signalChangeGroupColor( QString::fromStdString( o->getKeyString( ) ),
      QColor( static_cast<int>(color[0]), static_cast<int>(color[1]),
        static_cast<int>(color[2]) ) );
  }

  void MainWindow::receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o )
  {
    emit signalDestroyGroup( QString::fromStdString( o->getKeyString( ) ) );
  }
}

