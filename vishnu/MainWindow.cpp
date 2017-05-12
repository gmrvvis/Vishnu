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

#include "utils/Auxiliars.hpp"

namespace vishnu
{
  MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , _ui( new Ui::MainWindow )
    , _zeqSession( DEFAULT_ZEQ_SESSION )
  {
    _ui->setupUi( this );   

    connect( _ui->buttonLoadXml, SIGNAL( clicked( bool ) ), this, SLOT( buttonLoadXml_clicked( ) ) );

    addApp( APICOLAT, "../../../apicolat/build/bin/apicolat" );
    addApp( CLINT,    "../../../Clint/build/bin/ClintExplorer" );
    addApp( SPINERET, "../../../spineret/build/bin/CellExplorer" );

    for ( const auto& app : _apps )
    {
      _ui->verticalLayout_2->addWidget( app.second->getPushButton( ) );
      connect( app.second->getPushButton( ), SIGNAL( clicked( bool ) ), this, SLOT( pushButtonApp_clicked( ) ) );
    }

    //TODO:
    //setMaximumHeight( height( ) );
    setFixedHeight( 500 );

    connect( this, SIGNAL( signalCreateGroup( const QString& ) ),
      SLOT( addGroup( const QString& ) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalChangeGroupName( const QString&, const QString& ) ),
      SLOT( changeGroupName( const QString&, const QString& ) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalRemoveGroup( const QString&, const bool& ) ),
      SLOT( removeGroup( const QString&, const bool& ) ), Qt::QueuedConnection );

    //Zeq
    manco::ZeqManager::instance( ).init( _zeqSession );
    std::this_thread::sleep_for( std::chrono::milliseconds(2500) );

    manco::ZeqManager::instance( ).setReceivedSyncGroupCallback(
      std::bind( &MainWindow::receivedSyncGroup, this, std::placeholders::_1 ) );

    manco::ZeqManager::instance( ).setReceivedChangeNameGroupUpdateCallback(
      std::bind( &MainWindow::receivedChangeNameGroupUpdate, this, std::placeholders::_1 ) );

    manco::ZeqManager::instance( ).setReceivedDestroyGroupCallback(
      std::bind( &MainWindow::receivedDestroyGroup, this, std::placeholders::_1 ) );
  }

  MainWindow::~MainWindow( )
  {
    delete _ui;
    for ( const auto& app : _apps )
    {
      if ( app.second->isOpen( ) )
      {
        app.second->terminate( );
      }
    }
  }

  void MainWindow::closeEvent( QCloseEvent * )
  {
    /*QMessageBox::StandardButton result = QMessageBox::question( this,
      QString( "Launcher" ), QString( "Are you sure?\n" ),
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes );

    if ( result != QMessageBox::Yes )
    {
      event->ignore( );
    }
    else
    {
      event->accept( );
    }
    */
  }

  void MainWindow::buttonLoadXml_clicked( )
  {
    QString fileDialog = QFileDialog::getOpenFileName( this,
      QString( "Choose XML file" ), QString( "" ), QString( "XML-Files(*.xml)" ) );

    if ( !fileDialog.isEmpty( ) )
    {
     _ui->xmlFilename->setText( fileDialog );
      checkApps( );
    }
  }

  void MainWindow::checkApps( )
  {
    if ( !_ui->xmlFilename->text( ).isEmpty( ) )
    {
      for ( const auto& app : _apps )
      {
        //TODO: parse XML in order to enable buttons instead of enabling all of them
        app.second->getPushButton( )->setEnabled( true );
      }
    }
  }

  void MainWindow::app_closed( int exitCode, QProcess::ExitStatus exitStatus )
  {
    QProcess* process = qobject_cast< QProcess* >( sender( ) );
    if ( exitStatus == QProcess::CrashExit )
    {
      std::cerr << "Error: " << process->program( ).toStdString( ) << " crashed!" << std::endl;
    }
    else if ( exitCode != 0 )
    {
      std::cerr << "Error: " << process->program( ).toStdString( ) << " failed!" << std::endl;
    }
    else
    {
      std::cout <<  process->program( ).toStdString( ) << " closed successfully." << std::endl;
    }

    //Anyway button should be enabled again
    std::map< std::string, Application* >::iterator it;
    for ( it = _apps.begin( ); it != _apps.end( ); ++it )
    {
      if ( it->second->program( ) == process->program( ) )
      {
        it->second->setProgram( QString( ) );
        it->second->getPushButton( )->setEnabled( true );
        break;
      }
    }
  }

  void MainWindow::pushButtonApp_clicked( )
  {
    QPushButton* appButton = qobject_cast< QPushButton* >( sender( ) );

    if ( !appButton )
    {
      std::cerr << "Error: App not found!" << std::endl;
      return;
    }

    std::string appName;
    Application* app;
    std::map< std::string, Application* >::iterator it;
    for ( it = _apps.begin( ); it != _apps.end( ); ++it )
    {
      if ( it->second->getPushButton( ) == appButton)
      {
        appName = it->first;
        app = it->second;
        break;
      }
    }

    if ( appName == "" )
    {
      std::cerr << "Error: App not found!" << std::endl;
      return;
    }

    std::cout << "Opening " << appName << std::endl;

    QString program = QString::fromStdString( app->getShellExecPath( ) );
    QStringList arguments;
    arguments << "-z" << QString::fromStdString(_zeqSession );
    arguments << "-f" << _ui->xmlFilename->text( );

    app->getPushButton( )->setEnabled( false );

    app->setReadChannel( QProcess::StandardOutput );
    app->waitForReadyRead( );
    app->readAllStandardOutput( );
    app->start( program, arguments );

    if ( app->waitForStarted( 2000 ) )
    {
      QByteArray out_data = app->readAllStandardOutput( );
      QString out_string( out_data );
      std::cout << out_string.toStdString( ).c_str( ) << std::endl;
      connect( app, SIGNAL( finished ( int , QProcess::ExitStatus ) ),
        this, SLOT( app_closed( int , QProcess::ExitStatus ) ) );
    }
    else
    {
      std::cout << "Error: App not start in time!" << std::endl;
    }
  }

  void MainWindow::addGroup(const QString &qKey )
  {

    std::string key = qKey.toStdString( );
    auto syncGroup = _syncGroups.at( key );

    //Add group to widgets groups
    QLabel* nameLabel = new QLabel( QString::fromStdString( syncGroup->getName( ) ) );
    QLabel* ownerLabel = new QLabel( QString::fromStdString( syncGroup->getOwner( ) ) );
    QPushButton* closePushButton = new QPushButton( );
    closePushButton->setIcon( QIcon( ":/icons/iconClose.png" ) );
    _widgetsGroups[ key ] = new WidgetsGroup( nameLabel, ownerLabel, closePushButton );

    //Add group to grid layout
    _ui->groupsGridLayout->addWidget( nameLabel );
    _ui->groupsGridLayout->addWidget( ownerLabel );
    _ui->groupsGridLayout->addWidget( closePushButton );

    //Add close push button event
    connect( closePushButton, SIGNAL( clicked( bool ) ), this, SLOT( closeGroup_clicked( ) ) );
  }

  void MainWindow::closeGroup_clicked( )
  {
    // Checks for XML loaded
    if ( _ui->xmlFilename->text( ) == QString( "" ) )
    {
      std::cout << "Error closing group: XML file is not loaded." << std::endl;
      return;
    }

    auto closeButton = qobject_cast< QPushButton* >( sender( ) );
    std::map<std::string, WidgetsGroup*>::iterator it;
    std::string key;
    for ( it = _widgetsGroups.begin( ); it != _widgetsGroups.end( ); ++it )
    {
      if ( it->second->getClosePushButton( ) == closeButton )
      {
        key = it->first;
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

    auto app = _apps.find( syncGroup->getOwner( ) );
    if( app != _apps.end( ) )
    {
      if ( !app->second->getPushButton( )->isEnabled( ) )
      {
        std::cout << "Error closing group: " << syncGroup->getOwner( ) << " is not closed." << std::endl;
        return;
      }
    }
    else
    {
      std::cerr << "Error closing group: App not found!" << std::endl;
      return;
    }

    // If XML loaded && owner app closed
    // true asks for remove row group confirmation
    removeGroup( QString::fromStdString( key ), true );
  }

  void MainWindow::changeGroupName( const QString& qKey, const QString& qNewName )
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

    std::cout << "Group: " << key << ". Changed group name '" << name <<
      "'' to '" << newName << "' succesfully." << std::endl;
  }

  void MainWindow::removeGroup( const QString& qKey, const bool& askFirst )
  {

    if ( askFirst )
    {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, "Remove group", "Do you want to remove '" +
        qKey + "'' group?", QMessageBox::Yes|QMessageBox::No);
      if ( reply != QMessageBox::Yes )
      {
        return;
      }
    }

    std::string key = qKey.toStdString( );

    auto widget = _widgetsGroups.at( key )->getNameLabel( );

    int index = _ui->groupsGridLayout->indexOf( widget );
    auto gp = qMakePair( -1, -1 );
    int rs, cs;
    _ui->groupsGridLayout->getItemPosition( index, &gp.first, &gp.second, &rs, &cs );
    unsigned int row = gp.first;

    // Remove row from grid
    Auxiliars::removeRow( _ui->groupsGridLayout, row );

    // Remove group from widgets
    _widgetsGroups.erase( key );

    // Remove group from map
    _syncGroups.erase( key );

    //Send publish destroy group event
    manco::ZeqManager::instance( ).publishDestroyGroup( key );

    std::cout << "Group: " << key << " removed successfully." << std::endl;
  }

  void MainWindow::addApp( const std::string& appName, const std::string& appPath )
  {
    std::pair<std::map<std::string, Application*>::iterator, bool> res =
      _apps.insert(std::make_pair( appName, new Application( appName, appPath ) ) );
    if ( !res.second )
    {
      std::cerr << "Error adding app: " << appName << " already exists!" << std::endl;
    }
    else
    {
      std::cout << appName << " added successfully." << std::endl;
    }
  }

  void MainWindow::receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o )
  {
    std::vector<unsigned int> color = o->getColorVector( );
    std::cout << "Received SyncGroup (" <<
      "\n\tkey: " << o->getKeyString( ) <<
      "\n\tname: " << o->getNameString( ) <<
      "\n\towner: " << o->getOwnerString( ) <<
      "\n\tcolor: (" << color[0] << ", " << color[1] << ", "  << color[2] << ")" <<
      "\n\tids: ";
    std::vector< std::string > vectorIds = manco::ZeqManager::split( o->getIdsString( ), DELIMITER );
    for ( auto it = vectorIds.begin( ); it != vectorIds.end( ); ++it )
    {
      std::cout << *it << ' ';
    }
    std::cout << ")" << std::endl;

    SyncGroup* syncGroup = new SyncGroup( o->getKeyString( ), o->getNameString( ),
      o->getOwnerString( ), vectorIds, color[0], color[1], color[2] );

    //Add group to sync groups
    _syncGroups[ o->getKeyString( ) ]= syncGroup;
    emit signalCreateGroup( QString::fromStdString( syncGroup->getKey( ) ) );
  }

  void MainWindow::receivedChangeNameGroupUpdate( zeroeq::gmrv::ConstChangeNameGroupPtr o )
  {
    std::string key = o->getKeyString( );
    std::cout << "Received ChangeNameGroup (" << key << ")" << std::endl;
    auto syncGroup = _syncGroups.at( key );
    if ( syncGroup == nullptr )
    {
      std::cerr << "Info: " << key << " group doesn't exist, ignoring change name group callback." << std::endl;
      return;
    }
    emit signalChangeGroupName( QString::fromStdString( syncGroup->getKey( ) ),
      QString::fromStdString( o->getNameString( ) ) );
  }

  void MainWindow::receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o )
  {
    std::string key = o->getKeyString( );
    std::cout << "Received DestroyGroup (" << key << ")" << std::endl;
    auto syncGroup = _syncGroups.at( key );
    if ( syncGroup == nullptr )
    {
      std::cerr << "Info: " << key << " group doesn't exist, ignoring destroy group callback." << std::endl;
      return;
    }
    emit signalRemoveGroup( QString::fromStdString( key ), false );
  }
}

