/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

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

#include <QGraphicsBlurEffect>

#include <iostream>
#include <stdio.h>

#include "Definitions.hpp"
#include "RegExpInputDialog.h"

#include "model/UserDataSets.h"
#include "widgets/UserDataSetListWidget.h"
#include "widgets/ZEQGroupListWidget.h"

#include "model/AppsConfig.h"

#include "DataSetWindow.h"

namespace vishnu
{
  MainWindow::MainWindow( sp1common::Args args,
    QWidget *parent )
    : QMainWindow( parent )
  {
    //Get User Preferences and Args
    std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );

    QString qUserDataFolder = QString::fromStdString( userDataFolder );
    QDir qDir( qUserDataFolder );
    if ( !qDir.exists( ) )
    {
      qDir.mkpath( qUserDataFolder );
    }

    std::string userPreferencesFile = userDataFolder + FILE_USER_PREFERENCES;

    _userPreferences =
      sp1common::JSON::deserialize< UserPreferences >( userPreferencesFile );

    if ( args.has( "-wd" ) )
    {
      _userPreferences->addUserPreference(
        "workingDirectory", args.get( "-wd" ) );
    }
    if ( args.has( "-z" ) )
    {
      _userPreferences->addUserPreference( "zeqSession", args.get( "-z" ) );
    }

    //MenuBar
    /*QMenu *fileMenu = new QMenu(tr("&File"));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));
    QMenu *itemsMenu = new QMenu(tr("&DataSets"));
    //addDataSetAction = itemsMenu->addAction(tr("&Add DataSet"));
    //removeAction = itemsMenu->addAction(tr("&Remove Item"));
    //QAction *ascendingAction = itemsMenu->addAction(tr("Sort in &Ascending Order"));
    //QAction *descendingAction = itemsMenu->addAction(tr("Sort in &Descending Order"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);*/
    //End MenuBar

    //ToolBar
    QToolBar* toolBar = new QToolBar( );
    toolBar->setMovable( false );
    toolBar->setContextMenuPolicy( Qt::PreventContextMenu );
    QAction* actionAddDataSet = new QAction( );
    actionAddDataSet->setIcon( QIcon( ":/icons/addDataSet.png" ) );
    actionAddDataSet->setText( "AddDataSet" );
    actionAddDataSet->setToolTip( "Add Data Set" );
    actionAddDataSet->setShortcut( QKeySequence( "Ctrl+O" ) );
    QObject::connect( actionAddDataSet, SIGNAL( triggered( ) ), this,
      SLOT( addDataSet( ) ) );
    toolBar->addAction( actionAddDataSet );
    addToolBar(Qt::TopToolBarArea, toolBar);
    //End ToolBar

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

    QVBoxLayout* mainVLayout = new QVBoxLayout( );
    centralWidget()->setLayout( mainVLayout );

    //UserDataSetListWidget
    _userDataSetListWidget.reset( new UserDataSetListWidget( ) );

    //Applications
    loadApps( );

    QVBoxLayout* appsVBoxLayout = new QVBoxLayout( );
    for ( const auto& application : _applications )
    {
      appsVBoxLayout->addWidget( application.second->getPushButton( ) );

      QObject::connect( application.second->getPushButton( ),
        SIGNAL( clicked( bool ) ), this, SLOT( runApp( ) ) );
    }

    //Groups
    _zeqGroupListWidget.reset( new ZEQGroupListWidget( ) );

    QSizePolicy groupsSizePolicy( QSizePolicy::Preferred,
      QSizePolicy::Expanding );
    groupsSizePolicy.setHorizontalStretch( 1 );
    _zeqGroupListWidget->setSizePolicy( groupsSizePolicy );

    //User DataSets
    QVBoxLayout* userDSLayout = new QVBoxLayout( );
    userDSLayout->addWidget( _userDataSetListWidget.get( ), 0 );
    mainVLayout->addLayout( userDSLayout, 2 );

    //Apps and groups
    QHBoxLayout* appsAndGroupsLayout = new QHBoxLayout( );

    //Applications
    QVBoxLayout* appsLayout = new QVBoxLayout( );
    appsLayout->addLayout( appsVBoxLayout, 1 );

    //ZEQ Groups
    QVBoxLayout* zeqLayout = new QVBoxLayout( );
    zeqLayout->addWidget( _zeqGroupListWidget.get( ), 0 );

    appsAndGroupsLayout->addLayout( appsLayout, 0 );
    appsAndGroupsLayout->addLayout( zeqLayout, 1 );
    mainVLayout->addLayout( appsAndGroupsLayout, 1 );

    //Status bar
    statusBar()->showMessage( "" );

    QObject::connect( this, SIGNAL( signalReloadDataSets( ) ), this,
      SLOT( slotReloadDataSets( ) ) );

    emit signalReloadDataSets( );

    checkApps( false );
/*
    //TEST DATASETS
    UserDataSetWidgetPtr userDataSetWidget2 =
      _userDataSetListWidget->addUserDataSet( "dataSet001",
      "/media/DATA/data/test", "dataSet.csv", "dataSet.json", "dataSet.xml",
      false );

    QObject::connect( userDataSetWidget2, SIGNAL( signalRemoveSelected( ) ),
      this, SLOT( slotRemoveUserDataSet( ) ) );

    QObject::connect( userDataSetWidget2, SIGNAL( signalCheckSelected( bool ) ),
      this, SLOT( slotCheckUserDataSet( bool ) ) );

    UserDataSetWidgetPtr userDataSetWidget3 =
      _userDataSetListWidget->addUserDataSet( "dataSet502",
      "/media/DATA/data/ds", "segmentations.csv", "test.json", "pyramidal.xml",
      false );

    QObject::connect( userDataSetWidget3, SIGNAL( signalRemoveSelected( ) ),
      this, SLOT( slotRemoveUserDataSet( ) ) );

    QObject::connect( userDataSetWidget3, SIGNAL( signalCheckSelected( bool ) ),
      this, SLOT( slotCheckUserDataSet( bool ) ) );
    //END TEST DATASETS


    //TEST GROUPS
    std::vector< std::string > ids;
    ids.emplace_back("test");
    syncGroup("GROUP1#!#CLINT", "GROUP1", "CLINT", ids, QColor(255, 65, 77));
    syncGroup("GROUP1#!#DCEXPLORER", "GROUP2", "DCEXPLORER", ids, QColor(3, 255, 77));
    syncGroup("GROUP2#!#DCEXPLORER", "GROUP3", "DCEXPLORER", ids, QColor(67, 67, 15));
    syncGroup("GROUP1#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(60, 128, 0));
    syncGroup("GROUP2#!#PYRAMIDAL", "GROUP3", "PYRAMIDAL", ids, QColor(127, 79, 55));
    changeGroupColor("GROUP1#!#CLINT", QColor(0, 255, 0));
    changeGroupName("GROUP2#!#DCEXPLORER", "newName");
    syncGroup("GROUP2#!#PYRAMIDAL", "otherName", "PYRAMIDAL", ids, QColor(255, 65, 77));
    removeGroup("GROUP3#!#PYRAMIDAL");
    //END TEST GROUPS
*/

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
    std::string userDataSetsFilename =
      qApp->applicationDirPath( ).toStdString( ) + std::string( "/" )
      + USER_DATA_FOLDER + std::string( "/" ) + FILE_DATASETS;

    if ( sp1common::Files::exist( userDataSetsFilename ) )
    {
      UserDataSetsPtr userDataSets =
        sp1common::JSON::deserialize< UserDataSets >( userDataSetsFilename );

      _userDataSetListWidget->clearDataSets( );

      for( const auto& userDataSet : userDataSets->getUserDataSets( ) )
      {
        UserDataSetWidgetPtr userDataSetWidget =
          _userDataSetListWidget->addUserDataSet( userDataSet->getName( ),
        userDataSet->getPath( ), userDataSet->getCsvFilename( ),
        userDataSet->getJsonFilename( ), userDataSet->getXmlFilename( ),
        userDataSet->getSelected( ) );

       QObject::connect( userDataSetWidget,
         SIGNAL( signalCheckSelected( bool ) ), this,
         SLOT( slotCheckUserDataSet( bool ) ) );

       QObject::connect( userDataSetWidget, SIGNAL( signalRemoveSelected( ) ),
         this, SLOT( slotRemoveUserDataSet( ) ) );
      }
    }
  }

  void MainWindow::addDataSet( void )
  {
    setBlurred( true );

    DataSetWindow* dataSetWindow = new DataSetWindow();

    dataSetWindow->setGeometry(
      QRect( 0, 0, APPLICATION_WIDTH, APPLICATION_HEIGHT ) );

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = ( screenGeometry.width() - dataSetWindow->width( ) ) / 2;
    int y = ( screenGeometry.height() - dataSetWindow->height( ) ) / 2;
    dataSetWindow->move( x, y );

    //dataSetWindow->setMinimumSize( 800, 800 );
    dataSetWindow->setMaximumSize( 800, 800 );
    dataSetWindow->setWindowIcon( QIcon( ":/icons/logoVishnu.png") );
    dataSetWindow->setWindowTitle( QApplication::applicationName( )
      + QString(" - Create new dataset" ));

    dataSetWindow->setModal( true );
    //int result = ;

    //If dataset created, create userdataset row
    if ( dataSetWindow->exec( ) == QDialog::Accepted )
    {
      UserDataSetPtr resultDataSet = dataSetWindow->getResultUserDataSet( );
      addUserDataSet( resultDataSet );
    }

    setBlurred( false );

    emit signalReloadDataSets( );
  }

  void MainWindow::slotRemoveUserDataSet( void )
  {
    QMessageBox::StandardButton reply = QMessageBox::warning( this,
      "Remove DataSet", "Do you want to remove '" + QString::fromStdString(
      _userDataSetListWidget->getCurrentDataSetName( ) ) + "'' DataSet?",
      QMessageBox::Yes|QMessageBox::No );
    if ( reply == QMessageBox::Yes )
    {
      //_userDataSetListWidget->removeCurrentDataSet( );
      removeUserDataSet( _userDataSetListWidget->getCurrentDataSet( ) );
      emit signalReloadDataSets( );
    }
  }

  void MainWindow::slotCheckUserDataSet( bool checked )
  {
    if ( checked )
    {
      _userDataSetListWidget->selectCurrentDataSet( );
    }
    checkApps( checked );
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

  void MainWindow::checkApps( bool checked )
  {
    for ( const auto& appplication : _applications )
    {
      appplication.second->getPushButton( )->setEnabled( checked );
    }
  }

  void MainWindow::loadApps( void )
  {
    std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );

    QString qUserDataFolder = QString::fromStdString( userDataFolder );
    QDir qDir( qUserDataFolder );
    if ( !qDir.exists( ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "User Data folder doesn't exist.", true );
    }

    std::string appsConfigFile = userDataFolder + FILE_APPS_CONFIG;
    if ( !sp1common::Files::exist( appsConfigFile ) )
    {
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
        "Apps Config file doesn't exist.", true );
    }

    AppsConfigPtr appsConfig
      = sp1common::JSON::deserialize< AppsConfig >( appsConfigFile );

    //Not const in order to add (or overwrite) other arguments
    for ( auto& application : appsConfig->getApplications( ) )
    {
      application->getArgs( ).set( "-z",
        _userPreferences->getUserPreference( "zeqSession" ) );

      std::string instanceId = sp1common::Strings::generateRandom( 5 );
      application->getArgs( ).set( "-id", instanceId );

      application->setWorkingDirectory(
        _userPreferences->getUserPreference( "workingDirectory" ) );

      AppProcessPtr appProcess( new AppProcess(
        application->getApplicationType( ), application->getDisplayName( ),
        application->getShellCommand( ), application->getArgs( ),
        application->getWorkingDirectory( ), application->getIconPath( ) ) );

      std::string owner = sp1common::toString(
        application->getApplicationType( ) ) + instanceId;

      _applications[ owner ] = appProcess;
    }
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
      if ( it.second->getPushButton( ) == appButton )
      {
        sp1common::Debug::consoleMessage( "Opening "
          + it.second->getDisplayName( ) );

        it.second->getPushButton( )->setEnabled( false );
        it.second->setReadChannel( QProcess::StandardOutput );
        //it.second->setProcessChannelMode(QProcess::ForwardedChannels); //DEBUG
        it.second->waitForReadyRead( );
        it.second->readAllStandardOutput( );

        QStringList arguments;
        for( const auto& arg : it.second->getArgs( ).get( ) )
        {
          arguments << QString::fromStdString( arg.first );
          if ( !arg.second.empty( ) )
          {
            arguments << QString::fromStdString( arg.second );
          }
        }

        std::string application = it.second->getShellCommand( );
        if ( !sp1common::Files::exist( application ) )
        {
          QMessageBox::information( this, QString( APPLICATION_NAME ),
            QString( "Can't find " ) + QString::fromStdString( application )
            + QString( " application." ), QMessageBox::Ok );

          sp1common::Error::throwError( sp1common::Error::ErrorType::Warning,
            "Can't find " + application + " application.", false );

          it.second->getPushButton( )->setEnabled( true );

          return;
        }

        it.second->start( QString::fromStdString( application ), arguments );

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
    std::string zeqSession =
      _userPreferences->getUserPreference( "zeqSession" );
    std::cout << "Init ZeroEQ Session ( " << zeqSession << " )" << std::endl;
    manco::ZeqManager::instance( ).init( zeqSession );
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

  void MainWindow::setBlurred( const bool& state )
  {
    if ( state )
    {
      QGraphicsBlurEffect* blur = new QGraphicsBlurEffect( this );
      setGraphicsEffect( blur );
      _userDataSetListWidget->setBlurred( true );
      _zeqGroupListWidget->setBlurred( true );
      statusBar( )->setGraphicsEffect( blur );
    }
    else
    {
      setGraphicsEffect( 0 );
      _userDataSetListWidget->setBlurred( false );
      _zeqGroupListWidget->setBlurred( false );
      statusBar( )->setGraphicsEffect( 0 );
    }
  }

  void MainWindow::addUserDataSet( const UserDataSetPtr& userDataSet )
  {
    UserDataSetsPtr userDataSets = readUserDataSets( );
    userDataSets->addUserDataSet( userDataSet );
    writeUserDataSets( userDataSets );
  }

  void MainWindow::removeUserDataSet( const UserDataSetPtr& userDataSet )
  {
    UserDataSetVector userDataSets = readUserDataSets( )->getUserDataSets( );
    UserDataSetsPtr newUserDataSets( new UserDataSets( ) );
    for ( size_t i = 0; i < userDataSets.size( ); ++i )
    {
      if ( userDataSet->getName( ) != userDataSets.at( i )->getName( ) )
      {
        newUserDataSets->addUserDataSet( userDataSets.at( i ) );
      }
    }
    writeUserDataSets( newUserDataSets );
  }

  UserDataSetsPtr MainWindow::readUserDataSets( void )
  {
    std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );
    std::string userDataSetsFilename = userDataFolder + FILE_DATASETS;
    UserDataSetsPtr userDataSets( new UserDataSets( ) );
    if ( sp1common::Files::exist( userDataSetsFilename ) )
    {
      userDataSets =
        sp1common::JSON::deserialize< UserDataSets >( userDataSetsFilename);
    }
    return userDataSets;
  }

  void MainWindow::writeUserDataSets( const UserDataSetsPtr& userDataSets )
  {
    std::string userDataFolder = qApp->applicationDirPath( ).toStdString( )
        + std::string( "/" ) + USER_DATA_FOLDER + std::string( "/" );
    std::string userDataSetsFilename = userDataFolder + FILE_DATASETS;
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
  }
}
