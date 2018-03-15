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

namespace vishnu
{
  MainWindow::MainWindow( sp1common::Args args,
    QWidget *parent )
    : QMainWindow( parent )
    , _ui( new Ui::MainWindow )
  {
    _ui->setupUi( this );

    _zeqSession = args.get( "-z" );

    _ui->xmlFilename->setText( QString::fromStdString( args.get( "-f" ) ) );
    checkApps( );

    connect( _ui->buttonLoadXml, SIGNAL( clicked( bool ) ), this,
      SLOT( buttonLoadXml_clicked( ) ) );

    loadClint();
    loadDCExplorer();
    loadPyramidal();

    for ( const auto& app : _apps )
    {
      _ui->verticalLayout_2->addWidget( app.second->getPushButton( ) );
      connect( app.second->getPushButton( ), SIGNAL( clicked( bool ) ), this,
        SLOT( pushButtonApp_clicked( ) ) );
    }

    //TODO: fix height
    //setMaximumHeight( height( ) );
    setFixedHeight( 500 );

    connect( this, SIGNAL( signalCreateGroup( const QString& ) ),
      SLOT( createGroup( const QString& ) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalChangeGroupName( const QString&,
      const QString& ) ), SLOT( changeGroupName( const QString&,
      const QString& ) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalDestroyGroup( const QString& ) ),
      SLOT( destroyGroup( const QString& ) ), Qt::QueuedConnection );

    //ZeqSession
    initZeqSession( );
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
    /*
    QMessageBox::StandardButton result = QMessageBox::question( this,
      QString( "Launcher" ), QString( "Are you sure?\n" ),
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
      QMessageBox::Yes );

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
      QString( "Choose XML file" ), QString( "" ),
      QString( "XML-Files(*.xml)" ) );

    if ( !fileDialog.isEmpty( ) )
    {
     _ui->xmlFilename->setText( fileDialog );
      checkApps( );
    }
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
    for (  auto app : _apps )
    {
      if ( app.second->program( ) == qProcess->program( ) )
      {
        //app.second->setProgram( QString( ) );
        disconnect( app.second, SIGNAL( finished ( int ,
          QProcess::ExitStatus ) ), this,
          SLOT( app_closed( int, QProcess::ExitStatus ) ) );
        app.second->getPushButton( )->setEnabled( true );
        break;
      }
    }
  }

  void MainWindow::checkApps( )
  {
    if ( !_ui->xmlFilename->text( ).isEmpty( ) )
    {                
      for ( const auto& app : _apps )
      {
        std::map<std::string, std::string> args = app.second->getArguments();
        auto it = args.find("-f");
        if ( it != args.end( ) )
        {
          app.second->setArgument( "-f",
            _ui->xmlFilename->text().toStdString( ) );
        }

        //TODO: parse XML in order to enable buttons instead of
        //enabling all of them
        app.second->getPushButton( )->setEnabled( true );
      }
    }
  }

  void MainWindow::loadClint()
  {
    std::string clintExplorerShellCommand = "../bin/ClintExplorer";
    std::map<std::string, std::string> clintExplorerArgs;
    clintExplorerArgs[ _zeqSession ] = ""; //TODO: send -z first
    std::string clintExplorerWD = "../";

    Application* clintApp = new Application( manco::ApplicationType::CLINT,
      clintExplorerShellCommand, clintExplorerArgs, clintExplorerWD );

    _apps[manco::ApplicationType::CLINT] = clintApp;
  }

  void MainWindow::loadDCExplorer()
  {
    std::string wecoShellCommand = "../bin/WeCo";
    std::map<std::string, std::string> wecoArgs;
    wecoArgs["-p"] = "12346";
    wecoArgs["-z"] = _zeqSession;
    std::string wecoWD = "../";

    Application* apicolatApp = new Application(
      manco::ApplicationType::DCEXPLORER, wecoShellCommand,
      wecoArgs, wecoWD );

    _apps[manco::ApplicationType::DCEXPLORER] = apicolatApp;
  }

  void MainWindow::loadPyramidal()
  {
    std::string spineretShellCommand = "../bin/CellExplorer";
    std::map<std::string, std::string> spineretArgs;
    spineretArgs["-z"] = _zeqSession;
    spineretArgs["-f"] = _ui->xmlFilename->text( ).toStdString( );
    std::string spineretWD = "../";

    Application* spineretApp = new Application(
      manco::ApplicationType::PYRAMIDAL, spineretShellCommand, spineretArgs,
      spineretWD );

    _apps[manco::ApplicationType::PYRAMIDAL] = spineretApp;
  }


  void MainWindow::pushButtonApp_clicked( )
  {
    QPushButton* appButton = qobject_cast< QPushButton* >( sender( ) );
    if ( !appButton )
    {
      std::cerr << "Error: App not found!" << std::endl;
      return;
    }
    manco::ApplicationType appName;
    for (const auto& it : _apps )
    {
      if ( it.second->getPushButton( ) == appButton)
      {        
        appName = it.first;

        Auxiliars::consoleDebugMessage( "Opening " + toString( appName ) );

        it.second->getPushButton( )->setEnabled( false );
        it.second->setReadChannel( QProcess::StandardOutput );
        //it.second->setProcessChannelMode(QProcess::ForwardedChannels); //DEBUG
        it.second->waitForReadyRead( );
        it.second->readAllStandardOutput( );

        QStringList arguments;
        for(const auto& arg : it.second->getArguments( ) )
        {
          arguments << QString::fromStdString( arg.first );
          if ( arg.second != "" )
          {
            arguments << QString::fromStdString( arg.second );
          }
        }

        it.second->start(
          QString::fromStdString( it.second->getShellCommand( ) ),
          arguments
        );

        QByteArray out_data = it.second->readAllStandardOutput( );
        QString out_string( out_data );
        std::cout << out_string.toStdString( ).c_str( ) << std::endl;
        connect( it.second, SIGNAL( finished ( int , QProcess::ExitStatus ) ),
          this, SLOT( app_closed( int , QProcess::ExitStatus ) ) );
        break;
      }
    }
  }

  void MainWindow::removeGroup_clicked( )
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

    manco::ApplicationType appType = manco::toApplicationType(
      syncGroup->getOwner( ) );
    auto app = _apps.find( appType );
    if( app != _apps.end( ) )
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

    _widgetsGroups[ key ] = new WidgetsGroup( nameLabel, ownerLabel,
      closePushButton );

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

    Auxiliars::consoleDebugMessage( "Group: '" + key +
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

    Auxiliars::consoleDebugMessage("Group: '" + key +
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

    Auxiliars::consoleDebugMessage( message );

    /*"\n\tids: ";
    for ( auto it = vectorIds.begin( ); it != vectorIds.end( ); ++it )
    {
      std::cout << *it << ' ';
    }*/
    //std::cout << ")" << std::endl;

    std::string key = o->getKeyString( );
    SyncGroup* syncGroup = new SyncGroup( key, o->getNameString( ),
      o->getOwnerString( ), vectorIds, color[0], color[1], color[2] );

    //Add or set group to sync groups

    std::map<std::string, SyncGroup*>::iterator it;
    it = _syncGroups.find( key );
    if ( it != _syncGroups.end( ) )
    {
      //Found -> Set group
      _syncGroups[ key ]= syncGroup;

      //Set widgets group     
      emit signalChangeGroupName( QString::fromStdString(
        syncGroup->getKey( ) ), QString::fromStdString( o->getNameString( ) ) );

      Auxiliars::consoleDebugMessage("Group: '" + key +
        "' changed successfully.");
    }
    else
    {
      //Not found -> Create new group
      _syncGroups[ o->getKeyString( ) ]= syncGroup;

      //Add widgets group
      emit signalCreateGroup( QString::fromStdString( syncGroup->getKey( ) ) );

      Auxiliars::consoleDebugMessage("Group: '" + key +
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
      Auxiliars::consoleDebugMessage("Group: '" + key + "' doesn't exist, " +
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
      Auxiliars::consoleDebugMessage("Group: '" + key + "' doesn't exist, " +
        "ignoring destroy group callback.");
      return;
    }
    emit signalDestroyGroup( QString::fromStdString( key ) );
  }

}

