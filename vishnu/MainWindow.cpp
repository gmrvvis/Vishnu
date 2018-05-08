#include "MainWindow.h"
#include "ui_MainWindow.h"

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
    , _ui( new Ui::MainWindow )
  {
    _ui->setupUi( this );

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

    QObject::connect( this, SIGNAL( signalSyncGroup( const QString&,
      const QString&, const QString&, const std::vector< std::string >&,
      const QColor& ) ), SLOT( syncGroup( const QString&, const QString&,
      const QString&, const std::vector< std::string >&, const QColor& ) ),
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

    QGridLayout* gridLayout = new QGridLayout( );
    _ui->centralWidget->setLayout( gridLayout );

    //DataSets && Properties horizontal layout
    QHBoxLayout* hBoxLayout = new QHBoxLayout();

    //DataSetListWidget
    _dataSetListWidget = new DataSetListWidget( );

    QSizePolicy dsSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    dsSizePolicy.setHorizontalStretch( 1 );
    _dataSetListWidget->setSizePolicy( dsSizePolicy );
    hBoxLayout->addWidget( _dataSetListWidget, 0 );

    QObject::connect( addDataSetAction, SIGNAL( triggered( ) ), this,
      SLOT( addDataSetItem( ) ) );

    QObject::connect( _dataSetListWidget,
      SIGNAL( addDataSetEvent( const std::string& ) ), this,
      SLOT( addDataSetItem( const std::string& ) ) );

    //PropertiesTableWidget
    _propertiesTableWidget = new PropertiesTableWidget( );

    QSizePolicy prSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    prSizePolicy.setHorizontalStretch( 1 );
    _propertiesTableWidget->setSizePolicy( prSizePolicy );
    hBoxLayout->addWidget( _propertiesTableWidget, 1 );

    gridLayout->addLayout( hBoxLayout, 0, 0, Qt::AlignTop );

    //Applications
    loadClint();
    loadDCExplorer();
    loadPyramidal();

    QHBoxLayout* hBoxLayout2 = new QHBoxLayout( );
    QVBoxLayout* vBoxLayout = new QVBoxLayout( );
    QGroupBox* appsGroupBox = new QGroupBox();
    for ( const auto& application : _applications )
    {
      vBoxLayout->addWidget( application.second->getPushButton( ) );

      QObject::connect( application.second->getPushButton( ),
        SIGNAL( clicked( bool ) ), this, SLOT( runApp( ) ) );
    }
    vBoxLayout->stretch(1);
    appsGroupBox->setLayout(vBoxLayout);

    QSizePolicy appsSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    appsSizePolicy.setHorizontalStretch( 1 );
    appsGroupBox->setSizePolicy( appsSizePolicy );
    hBoxLayout2->addWidget( appsGroupBox, 0 );

    //Groups
    _zeqGroupListWidget = new ZEQGroupListWidget( );

    QSizePolicy groupsSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    groupsSizePolicy.setHorizontalStretch( 1 );
    _zeqGroupListWidget->setSizePolicy( groupsSizePolicy );
    hBoxLayout2->addWidget( _zeqGroupListWidget, 1 );

    gridLayout->addLayout( hBoxLayout2, 1, 0, Qt::AlignTop );

    std::vector<std::string> ids;
    ids.push_back("test");
    syncGroup("GROUP1#!#CLINT", "GROUP1", "CLINT", ids, QColor(255, 65, 77));
    syncGroup("GROUP1#!#DCEXPLORER", "GROUP2", "DCEXPLORER", ids, QColor(3, 255, 77));
    syncGroup("GROUP2#!#DCEXPLORER", "GROUP3", "DCEXPLORER", ids, QColor(67, 67, 15));
    syncGroup("GROUP1#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(60, 128, 0));
    syncGroup("GROUP2#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(127, 79, 55));
    changeGroupColor("GROUP1#!#CLINT", QColor(0, 255, 0));
    changeGroupName("GROUP2#!#DCEXPLORER", "newName");
    syncGroup("GROUP2#!#PYRAMIDAL", "otherName", "PYRAMIDAL", ids, QColor(255, 65, 77));
    removeGroup("GROUP3#!#PYRAMIDAL");
  }

  MainWindow::~MainWindow( )
  {
    delete _ui;
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
    }
  }

  void MainWindow::removeDataSetItem( )
  {
    std::vector< std::string > propertiesToRemove =
      _dataSetListWidget->getPropertiesToRemove( );

    _propertiesTableWidget->removeProperties( propertiesToRemove );

    _dataSetListWidget->removeCurrentDataSet( );
  }

  void MainWindow::syncGroup( const QString& key, const QString& name,
    const QString& owner, const std::vector< std::string >& ids,
    const QColor& color )
  {
    ZEQGroupWidget* zeqGroupWidget = _zeqGroupListWidget->syncGroup(
      key.toStdString( ), name, owner, ids, color );

    if ( zeqGroupWidget )
    {
      QObject::connect( zeqGroupWidget, SIGNAL( signalChangeGroupName( const QString&, const QString& ) ), this,
        SLOT( changeGroupName( const QString&, const QString& ) ) );

      QObject::connect( zeqGroupWidget, SIGNAL( signalChangeGroupColor( const QString&, const QColor& ) ), this,
        SLOT( changeGroupColor( const QString&, const QColor& ) ) );

      QObject::connect( zeqGroupWidget, SIGNAL( signalRemoveSelectedGroup( ) ), this,
        SLOT( removeSelectedGroup( ) ) );
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
    std::vector< std::string > properties;
    //Properties properties;

    for ( auto const& property : _propertiesTableWidget->getProperties( ) )
    {
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
          //properties.push_back( property );
          properties.push_back( property->getName( ) );
        }
      }
    }

    if ( !pkInUse )
    {
       sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
         "There must be at least one primary key in properties table.", false );

       return false;
    }

    std::string primaryKeysJoined = sp1common::Strings::join( primaryKeys,
      "-" );

    //Create CSV with data
    std::string outFile = dir.absolutePath( ).toStdString( ) + "/data.csv";

    std::string headers = sp1common::Strings::join( properties, "," );
    headers = primaryKeysJoined + "," + headers;

    result = sp1common::Files::writeCsv( outFile, headers );
    if ( !result )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
        "Can't create CSV file.", false );
      return false;
    }

    for ( const auto& dataSet : _dataSetListWidget->getDataSets( ) )
    {
      if ( dataSet.second->getChecked( ) )
      {
        sp1common::Matrix csvData = sp1common::Files::readCsv(
          dataSet.second->getPath( ) );

        std::vector< unsigned int > primaryKeyIndices;
        std::vector< std::string > dataSetHeaders = dataSet.second->getHeaders( );
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
              pkLine += "-" + csvRow.at( col );
            }
            else
            {
              line += (line.empty()) ? csvRow.at( col ) : "," + csvRow.at( col );
            }
          }

          line = dataSet.first + pkLine + "," + line;
          result = sp1common::Files::writeCsv( outFile, line, true );
          if ( !result )
          {
            sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
              "Can't create CSV file.", false );
            return false;
          }
        }
      }
    }

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
      QIcon(":/icons/logoClint.jpeg"), _workingDirectory ) );

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
      QIcon(":/icons/logoDCExplorer.ico"), _workingDirectory ) );

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

