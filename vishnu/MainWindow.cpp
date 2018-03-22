#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPixmap>

#include <iostream>
#include <stdio.h>

#include "Definitions.hpp"
#include "utils/Auxiliars.hpp"
#include "RegExpInputDialog.hpp"
#include "widgets/DataSetWidget.hpp"

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

    //DataSetListWidget
    //EndDataSetListWidget



    _workingDirectory = args.get( "-wd" );
    _zeqSession = args.get( "-z" );



    loadClint();
    loadDCExplorer();
    loadPyramidal();

    for ( const auto& application : _applications )
    {
      _ui->verticalLayout_2->addWidget( application.second->getPushButton( ) );
      QObject::connect( application.second->getPushButton( ),
        SIGNAL( clicked( bool ) ), this, SLOT( pushButtonApp_clicked( ) ) );
    }

    QObject::connect( this, SIGNAL( signalCreateGroup( const QString& ) ),
      SLOT( createGroup( const QString& ) ), Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signalChangeGroupName( const QString&,
      const QString& ) ), SLOT( changeGroupName( const QString&,
      const QString& ) ), Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signalDestroyGroup( const QString& ) ),
      SLOT( destroyGroup( const QString& ) ), Qt::QueuedConnection );

    //ZeqSession
    initZeqSession( );

    //DataSetListWidget
    //_ui->dataSetListWidget = new QListWidget(this);
    //_ui->dataSetListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(addDataSetAction, SIGNAL(triggered()), this, SLOT(addDataSetItem()));
    //QObject::connect(removeDataSetAction, SIGNAL(triggered()), this, SLOT(removeDataSetItem()));
    //QObject::connect(listWidget,
     //       SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
      //      this, SLOT(updateMenus(QListWidgetItem *)));

    //End DataSetListWidget

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

  void MainWindow::addDataSetItem( )
  {
    QString filePath = QFileDialog::getOpenFileName( this,
      QString( "Choose CSV file" ), QString( "" ),
      QString( "CSV-Files(*.csv)" ) );

    if ( filePath.isEmpty( ) )
    {
      return;
    }

    QFileInfo fileInfo( filePath );

    //Format basename to 10 chars without spaces
    std::string basename = fileInfo.baseName( ).toStdString( );
    basename.erase( std::remove_if( basename.begin( ), basename.end( ),
      isspace ), basename.end( ) );
    basename.resize (10);

    std::string name;
    std::string path = filePath.toStdString( );
    std::string tempName = basename;
    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      QString dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name", QString::fromStdString( tempName ),
        regularExpression, &validName);
      if ( !validName )
      {
        return;
      }

      name = dataSetName.toStdString( );

      //Check if name doesn't exist
      notUsedName = true;
      for(int i = 0; i < _ui->dataSetListWidget->count(); ++i)
      {
        QListWidgetItem* item = _ui->dataSetListWidget->item( i );
        DataSetWidget* widget = static_cast< DataSetWidget* >(
          _ui->dataSetListWidget->itemWidget( item ) );
        if ( widget->getName( ) == name )
        {
          QMessageBox::warning( this, APPLICATION_NAME,
            tr("Invalid name. This name already exists.") );
          notUsedName = false;
        }
      }

    } while( !notUsedName );

    //Add to dataset
    DataSetWidget* dsw = new DataSetWidget( name, path );    
    QListWidgetItem* listWidgetItem = new QListWidgetItem(
      _ui->dataSetListWidget );
    _ui->dataSetListWidget->addItem( listWidgetItem );
    listWidgetItem->setSizeHint( dsw->sizeHint ( ) );
    _ui->dataSetListWidget->setItemWidget (listWidgetItem, dsw);

    QObject::connect( dsw->getPushButton( ), SIGNAL( clicked( ) ), this,
      SLOT( removeDataSetItem( ) ) );

    //dsw->getPushButton( )->parent( )
    std::vector< std::string > csvHeaders =
      sp1common::Files::readCsvHeaders( filePath.toStdString( ) );

    std::cout << sp1common::Strings::join( csvHeaders, "," ) << std::endl;

    //_ui->csvFilename->setText( fileDialog );
    //checkApps( );
  }


  void MainWindow::removeDataSetItem( )
  {
    _ui->dataSetListWidget->takeItem( _ui->dataSetListWidget->row(
      _ui->dataSetListWidget->currentItem()));
  }

  void MainWindow::app_closed( int exitCode, QProcess::ExitStatus exitStatus )
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
          SLOT( app_closed( int, QProcess::ExitStatus ) ) );
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

    ApplicationPtr appplication( new Application( manco::ApplicationType::CLINT,
      displayName, cmd, args, _workingDirectory ) );

    std::string owner = manco::ZeqManager::getOwner(
      manco::ApplicationType::CLINT, instanceId );

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
      manco::ApplicationType::DCEXPLORER, displayName, cmd, args,
      _workingDirectory ) );

    std::string owner = manco::ZeqManager::getOwner(
      manco::ApplicationType::DCEXPLORER, instanceId );

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

    ApplicationPtr application( new Application( manco::ApplicationType::PYRAMIDAL,
      displayName, cmd, args, _workingDirectory ) );

    std::string owner = manco::ZeqManager::getOwner(
      manco::ApplicationType::PYRAMIDAL, instanceId );

    _applications[ owner ] = application;
  }

  void MainWindow::pushButtonApp_clicked( )
  {
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
        sp1common::Debug::consoleMessage( "Opening " + it.second->getDisplayName( ) );

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
          SLOT( app_closed( int , QProcess::ExitStatus ) ) );
        break;
      }
    }
  }

  void MainWindow::removeGroup_clicked( )
  {
    auto closeButton = qobject_cast< QPushButton* >( sender( ) );
    std::string key;
    for ( const auto& it : _widgetsGroups )
    {
      if ( it.second->getClosePushButton( ) == closeButton )
      {
        key = it.first;
        break;
      }
    }

    if ( key == "" )
    {
      std::cerr << "Error closing group: Group not found!" << std::endl;
      return;
    }

    // Checks for app closed
    auto syncGroup = _syncGroups.at( key );

    std::string owner = syncGroup->getOwner( );

    auto app = _applications.find( owner );
    if( app != _applications.end( ) )
    {
      if ( !app->second->getPushButton( )->isEnabled( ) )
      {
        std::cout << "Error destroying group: " << syncGroup->getOwner( ) <<
          " is not closed." << std::endl;
        QMessageBox::warning( this, "Error destroying group",
          "Error destroying group: " + QString::fromStdString(
          syncGroup->getOwner( ) ) + " is not closed.", QMessageBox::Ok );
        return;
      }
    }
    else
    {
      std::cerr << "Error closing group: App not found!" << std::endl;
      return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning( this, "Remove group",
      "Do you want to remove '" + QString::fromStdString( key ) + "'' group?",
      QMessageBox::Yes|QMessageBox::No );
    if ( reply == QMessageBox::Yes )
    {
      destroyGroup( QString::fromStdString( key ) );
      manco::ZeqManager::instance( ).publishDestroyGroup( key );
    }
  }

  void MainWindow::createGroup( const QString &qKey )
  {
    std::string key = qKey.toStdString( );
    auto syncGroup = _syncGroups.at( key );   

    //Add group to widgets groups
    QLabel* nameLabel = new QLabel(
      QString::fromStdString( syncGroup->getName( ) ) );
    QLabel* ownerLabel = new QLabel(
      QString::fromStdString( syncGroup->getOwner( ) ) );
    QPushButton* closePushButton = new QPushButton( );
    closePushButton->setIcon( QIcon( ":/icons/iconClose.png" ) );

    _widgetsGroups[ key ] = WidgetsGroupPtr( new WidgetsGroup( nameLabel,
      ownerLabel, closePushButton ) );

    //Add group to grid layout
    _ui->groupsGridLayout->addWidget( nameLabel );
    _ui->groupsGridLayout->addWidget( ownerLabel );
    _ui->groupsGridLayout->addWidget( closePushButton );

    //Add close push button event
    connect( closePushButton, SIGNAL( clicked( bool ) ), this,
      SLOT( removeGroup_clicked( ) ) );
  }

  void MainWindow::changeGroupName( const QString& qKey,
    const QString& qNewName )
  {
    std::string key = qKey.toStdString( );
    std::string newName = qNewName.toStdString( );

    //Sync groups
    auto syncGroup = _syncGroups.at( key );
    std::string name = syncGroup->getName( );
    syncGroup->setName( newName );

    //Widgets groups
    auto widgetsGroup = _widgetsGroups.at( key );
    QLabel* nameLabel = widgetsGroup->getNameLabel( );
    nameLabel->setText( qNewName );

    sp1common::Debug::consoleMessage( "Group: '" + key +
      "' - Changed group name '" + name + "'' to '" + newName +
      "' successfully." );
  }

  void MainWindow::destroyGroup( const QString& qKey )
  {
    std::string key = qKey.toStdString( );
    auto widget = _widgetsGroups.at( key )->getNameLabel( );
    int index = _ui->groupsGridLayout->indexOf( widget );
    auto gp = qMakePair( -1, -1 );
    int rs, cs;
    _ui->groupsGridLayout->getItemPosition( index, &gp.first, &gp.second, &rs,
      &cs );
    int row = gp.first;

    // Remove row from grid
    Auxiliars::removeRow( _ui->groupsGridLayout, row );

    // Remove group from widgets
    _widgetsGroups.erase( key );

    // Remove group from map
    _syncGroups.erase( key );

    sp1common::Debug::consoleMessage("Group: '" + key +
      "' removed successfully.");
  }

  void MainWindow::initZeqSession( )
  {
    std::cout << "Init ZeqSession ( " << _zeqSession << " )" << std::endl;
    manco::ZeqManager::instance( ).init( _zeqSession );
    manco::ZeqManager::instance( ).setReceivedSyncGroupCallback(
      std::bind( &MainWindow::receivedSyncGroup, this,
      std::placeholders::_1 ) );
    manco::ZeqManager::instance( ).setReceivedChangeNameGroupUpdateCallback(
      std::bind( &MainWindow::receivedChangeNameGroupUpdate, this,
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

    std::string message = std::string( "Received SyncGroup (" ) +
      std::string( "\n\tkey: " ) + o->getKeyString( ) +
      std::string( "\n\tname: " ) + o->getNameString( ) +
      std::string( "\n\towner: " ) + o->getOwnerString( ) +
      std::string( "\n\tcolor: (" ) +
      std::to_string( color[0] ) + std::string( ", " ) +
      std::to_string( color[1] ) + std::string( ", " ) +
      std::to_string( color[2] ) +
      std::string( ")" ) +
      std::string( "\n\tids: (... commented ...)" );

    sp1common::Debug::consoleMessage( message );

    /*"\n\tids: ";
    for ( auto it = vectorIds.begin( ); it != vectorIds.end( ); ++it )
    {
      std::cout << *it << ' ';
    }*/
    //std::cout << ")" << std::endl;

    std::string key = o->getKeyString( );
    SyncGroupPtr syncGroup( new SyncGroup( key, o->getNameString( ),
      o->getOwnerString( ), vectorIds, color[0], color[1], color[2] ) );

    //Add or set group to sync groups

    //SyncGroups::iterator it;
    auto it = _syncGroups.find( key );
    if ( it != _syncGroups.end( ) )
    {
      //Found -> Set group
      _syncGroups[ key ] = syncGroup;

      //Set widgets group     
      emit signalChangeGroupName( QString::fromStdString(
        syncGroup->getKey( ) ), QString::fromStdString( o->getNameString( ) ) );

      sp1common::Debug::consoleMessage("Group: '" + key +
        "' changed successfully.");
    }
    else
    {
      //Not found -> Create new group
      _syncGroups[ o->getKeyString( ) ]= syncGroup;

      //Add widgets group
      emit signalCreateGroup( QString::fromStdString( syncGroup->getKey( ) ) );

      sp1common::Debug::consoleMessage("Group: '" + key +
        "' created successfully.");
    }
  }

  void MainWindow::receivedChangeNameGroupUpdate(
    zeroeq::gmrv::ConstChangeNameGroupPtr o )
  {
    std::string key = o->getKeyString( );
    //std::cout << "DEBUG: Received ChangeNameGroupUpdate, key: " << key <<
    //  std::endl;
    auto syncGroup = _syncGroups[ key ];
    if ( syncGroup == nullptr )
    {
      sp1common::Debug::consoleMessage("Group: '" + key + "' doesn't exist, " +
        "ignoring change name group callback.");
      return;
    }

    emit signalChangeGroupName( QString::fromStdString( syncGroup->getKey( ) ),
      QString::fromStdString( o->getNameString( ) ) );
  }

  void MainWindow::receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o )
  {
    std::string key = o->getKeyString( );
    //std::cout << "DEBUG: Received DestroyGroup, key: " << key << std::endl;
    auto syncGroup = _syncGroups.find( key );
    if ( syncGroup == _syncGroups.end( ) )
    {
      sp1common::Debug::consoleMessage("Group: '" + key + "' doesn't exist, " +
        "ignoring destroy group callback.");
      return;
    }
    emit signalDestroyGroup( QString::fromStdString( key ) );
  }

}

