#include "MainWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
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

#include "widgets/UserDataSetListWidget.h"
#include "UserDataSets.h"
#include "widgets/ZEQGroupListWidget.h"

#include "DataSetWindow.h"

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
    //addDataSetAction = itemsMenu->addAction(tr("&Add DataSet"));
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
      SLOT( addDataSet( ) ) );
    /*QObject::connect( actionAddDataSet, SIGNAL( triggered() ), this,
      SLOT( addDataSetItem( ) ) );*/
    toolBar->addAction( actionAddDataSet );
    addToolBar(Qt::TopToolBarArea, toolBar);
    //End ToolBar

    _workingDirectory = args.get( "-wd" );
    _zeqSession = args.get( "-z" );

    qRegisterMetaType< std::vector< std::string > >("StringVector");

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

    //Central Widget
    setCentralWidget( new QWidget( ) );

    QHBoxLayout* mainHLayout = new QHBoxLayout( );
    centralWidget()->setLayout( mainHLayout );

    //UserDataSetListWidget
    _userDataSetListWidget.reset( new UserDataSetListWidget( ) );

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
    /*QSizePolicy dsSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    dsSizePolicy.setHorizontalStretch( 255 );
    _dataSetListWidget->setSizePolicy( dsSizePolicy );

    QSizePolicy prSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    prSizePolicy.setHorizontalStretch( 1 );
    _propertiesTableWidget->setSizePolicy( prSizePolicy );*/

    QSizePolicy groupsSizePolicy( QSizePolicy::Preferred,
      QSizePolicy::Expanding );
    groupsSizePolicy.setHorizontalStretch( 1 );
    _zeqGroupListWidget->setSizePolicy( groupsSizePolicy );

    //User DataSets
    QVBoxLayout* userDSLayout = new QVBoxLayout( );
    userDSLayout->addWidget( _userDataSetListWidget.get( ), 0 );
    mainHLayout->addLayout( userDSLayout, 0 );

    //Applications
    QVBoxLayout* appsLayout = new QVBoxLayout();
    appsLayout->addLayout( appsVBoxLayout, 0 );
    mainHLayout->addLayout( appsLayout, 1 );

    //ZEQ Groups
    QVBoxLayout* zeqLayout = new QVBoxLayout( );
    zeqLayout->addWidget( _zeqGroupListWidget.get( ), 0 );
    mainHLayout->addLayout( zeqLayout, 2 );

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

    QObject::connect( this, SIGNAL( signalReloadDataSets( ) ), this,
      SLOT( slotReloadDataSets( ) ) );

    emit signalReloadDataSets( );
  }

  MainWindow::~MainWindow( )
  {
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
      QString( APPLICATION_NAME ),
      QString( "Are you sure you want to quit?\n" ),
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


  void MainWindow::slotReloadDataSets( void )
  {
    std::string userDataSetsFilename = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" )
        + USER_DATASETS_FILENAME;

    if ( sp1common::Files::exist( userDataSetsFilename ) )
    {
      UserDataSetsPtr userDataSets =
        sp1common::JSON::deserialize< UserDataSets >( userDataSetsFilename );

      _userDataSetListWidget->clearDataSets( );

      /*UserDataSetWidgetPtr userDataSetWidget =
        _userDataSetListWidget->addUserDataSet( "NAME", "PATH", "CSV", "XML",
        false );*/

      for( const auto& userDataSet : userDataSets->getUserDataSets( ) )
      {
        UserDataSetWidgetPtr userDataSetWidget =
          _userDataSetListWidget->addUserDataSet( userDataSet->getName( ),
        userDataSet->getPath( ), userDataSet->getCsvFilename( ),
        userDataSet->getXmlFilename( ), userDataSet->getSelected( ) );

       QObject::connect( userDataSetWidget, SIGNAL( signalRemoveSelected( ) ),
         this, SLOT( slotRemoveUserDataSet( ) ) );
      }
    }
  }

  void MainWindow::addDataSet( void )
  {
    DataSetWindow* dataSetWindow = new DataSetWindow();

    dataSetWindow->setGeometry( QRect( 0, 0, APPLICATION_WIDTH, APPLICATION_HEIGHT ) );

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = ( screenGeometry.width() - dataSetWindow->width( ) ) / 2;
    int y = ( screenGeometry.height() - dataSetWindow->height( ) ) / 2;
    dataSetWindow->move( x, y );

    dataSetWindow->setMinimumSize( 800, 800 );
    dataSetWindow->setMaximumSize( 800, 800 );
    dataSetWindow->setWindowIcon( QIcon( ":/icons/logoVishnu.png") );
    dataSetWindow->setWindowTitle( QApplication::applicationName( )
      + QString(" - Create new dataset"));

    dataSetWindow->setModal( true );
    dataSetWindow->exec( );

    emit signalReloadDataSets( );
  }

  void MainWindow::slotRemoveUserDataSet( void )
  {
    _userDataSetListWidget->removeCurrentDataSet( );
  }

  void MainWindow::syncGroup( const QString& key, const QString& name,
    const QString& owner, const std::vector< std::string >& ids,
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
    QMessageBox::StandardButton reply = QMessageBox::warning( this,
      "Remove group", "Do you want to remove '"
      + QString::fromStdString( currentKey ) + "'' group?",
      QMessageBox::Yes|QMessageBox::No );
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
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        qProcess->program( ).toStdString( ) + " crashed!", false );
    }
    else if ( exitCode != 0 )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        qProcess->program( ).toStdString( ) + " failed!", false );
    }
    else
    {
      sp1common::Debug::consoleMessage( qProcess->program( ).toStdString( )
        + " closed successfully." );
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
    //TODO: Check if dataset is selected

    QPushButton* appButton = qobject_cast< QPushButton* >( sender( ) );
    if ( !appButton )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
       "Application not found!", true );
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

