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

#define ARG "hbp://"

namespace vishnu
{

  void MainWindow::receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o )
  {
    std::vector<unsigned int> color = o->getColorVector( );
    std::cout << "Received SyncGroup (" <<
                 "\n\tkey: " << o->getKeyString( ) <<
                 "\n\tname: " << o->getNameString( ) <<
                 "\n\towner: " << o->getOwnerString( ) <<
                 "\n\tcolor: (" << color[0] << ", " << color[1] << ", "  << color[2] << ")" <<
                 "\n\tids: "; //<< o->getIdsString( ) << ")";
    std::vector< std::string > vectorIds = manco::ZeqManager::split( o->getIdsString( ), DELIMITER );
    for ( auto it = vectorIds.begin(); it != vectorIds.end(); ++it )
    {
      std::cout << *it << ' ';
    }
    std::cout << ")" << std::endl;


    SyncGroup* syncGroup = new SyncGroup(
      o->getKeyString( ),
      o->getNameString( ),
      o->getOwnerString( ),
      vectorIds,
      color[0],
      color[1],
      color[2]
    );

    //Add group to sync groups
    _syncGroups[ o->getKeyString( ) ]= syncGroup;

    emit signalCreateGroup( QString::fromStdString( syncGroup->getKey( ) ) );
  }

  void MainWindow::receivedChangeNameGroupUpdate( zeroeq::gmrv::ConstChangeNameGroupPtr o)
  {
    std::string key = o->getKeyString( );

    std::cout << "Received ChangeNameGroup (" << key << ")" << std::endl;

    auto syncGroup = _syncGroups[ key ];
    if ( syncGroup == nullptr )
    {
        std::cerr << "Info: " << key << " group doesn't exist, ignoring change name group callback." << std::endl;
        return;
    }

    emit signalChangeGroupName( QString::fromStdString( syncGroup->getKey( ) ),
      QString::fromStdString( o->getNameString( ) ) );

  }

  void MainWindow::receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o)
  {
    std::string key = o->getKeyString( );

    std::cout << "Received DestroyGroup (" << key << ")" << std::endl;

    auto syncGroup = _syncGroups[ key ];
    if ( syncGroup == nullptr )
    {
        std::cerr << "Info: " << key << " group doesn't exist, ignoring destroy group callback." << std::endl;
        return;
    }

    removeRowGroup( key );

  }

  MainWindow::MainWindow( QWidget *parent )
      : QMainWindow( parent )
      , ui( new Ui::MainWindow )
  {
    ui->setupUi( this );

    connect( ui->buttonLoadXml, SIGNAL( clicked( bool ) ), this, SLOT( buttonLoadXml_clicked( ) ) );

    addAppToMap( APICOLAT, "../../../apicolat/build/bin/apicolat" );
    addAppToMap( CLINT,    "../../../Clint/build/bin/ClintExplorer" );
    addAppToMap( SPINERET, "../../../spineret/build/bin/CellExplorer" );

    for ( const auto& app : _loadedApps )
    {
      ui->verticalLayout_2->addWidget( app.second->getPushButton( ) );
      connect( app.second->getPushButton( ), SIGNAL( clicked( bool ) ), this, SLOT( pushButtonApp_clicked( ) ) );
    }

    //setMaximumHeight(height());
    setFixedHeight(500);

    connect(this, SIGNAL(signalCreateGroup( QString ) ),
            SLOT( addGroupToGrid( QString ) ), Qt::QueuedConnection);

    connect(this, SIGNAL(signalChangeGroupName( QString, QString ) ),
            SLOT( changeGroupName( QString, QString ) ), Qt::QueuedConnection);

    //Zeq
    manco::ZeqManager::instance( ).init( ARG );
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
      delete ui;

      for ( const auto& app : _loadedApps )
      {
        if ( app.second->isOpen( ) )
        {
            app.second->terminate( );

            QPushButton* button = ui->groupBox->findChild< QPushButton* >( QString::fromStdString( app.first ) );
            button->setEnabled( true );
        }
      }
  }


  void MainWindow::closeEvent( QCloseEvent * )
  {
      /*QMessageBox::StandardButton result = QMessageBox::question( this, QString( "Launcher" ),
                                                                  tr( "Are you sure?\n" ),
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
      QString fileDialog = QFileDialog::getOpenFileName( this, "Choose XML file",
                                                              "",
                                                              "XML-Files(*.xml)" );
      if ( !fileDialog.isEmpty( ) )
      {
          ui->xmlFilename->setText( fileDialog );
          checkAppsOK( );
      }
  }

  void MainWindow::checkAppsOK( )
  {
      if ( !ui->xmlFilename->text( ).isEmpty( ) )
      {
          //TODO: parse XML to enable buttons
          //QXmlSimpleReader xmlReader;
          //QXmlInputSource *source = new QXmlInputSource( ui->xmlFilename->text( ) );

          QPushButton* button = ui->groupBox->findChild< QPushButton* >( QString( "SPINERET" ) );
          if ( button )
          {
              button->setEnabled( true );
          }

          QPushButton* button2 = ui->groupBox->findChild< QPushButton* >( QString( "APICOLAT" ) );
          if ( button2 )
          {
              button2->setEnabled( true );
          }

          QPushButton* button3 = ui->groupBox->findChild< QPushButton* >( QString( "CLINT" ) );
          if ( button3 )
          {
              button3->setEnabled( true );
          }
      }
  }


  void MainWindow::app_closed( int exitCode, QProcess::ExitStatus exitStatus )
  {
    QProcess* process = qobject_cast< QProcess* >( sender( ) );
    if ( exitStatus == QProcess::CrashExit )
    {
        std::cerr << "Error: " << process->program().toStdString() << " crashed!" << std::endl;
    }
    else if ( exitCode != 0 )
    {
        std::cerr << "Error: " << process->program().toStdString() << " failed!" << std::endl;
    }
    else
    {
        std::cout <<  process->program().toStdString() << " closed successfully." << std::endl;
    }

    //Anyway button should be enabled again
    std::map< std::string, Application* >::iterator it;
    for ( it = _loadedApps.begin( ); it != _loadedApps.end( ); ++it )
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
    if ( appButton )
    {
      std::string appName = appButton->objectName( ).toStdString( );

      if ( _loadedApps.find( appName ) != _loadedApps.end( ) )
      {
        std::cout << "Opening... " << appButton->objectName().toStdString() << std::endl;

        QString program = QString::fromStdString( _loadedApps.at( appName )->getLauncherCmd() );
        QStringList arguments;
        arguments << "-z" << QString( ARG );
        arguments << "-f" << ui->xmlFilename->text( );

        appButton->setEnabled( false );

        _loadedApps.at( appName )->setReadChannel( QProcess::StandardOutput );
        _loadedApps.at( appName )->waitForReadyRead( );
        _loadedApps.at( appName )->readAllStandardOutput( );
        _loadedApps.at( appName )->start( program, arguments );

        if (_loadedApps.at( appName )->waitForStarted( 500 ) )
        {
          QByteArray out_data = _loadedApps.at( appName )->readAllStandardOutput( );
          QString out_string( out_data );
          std::cout << out_string.toStdString().c_str( ) << std::endl;
          connect( _loadedApps.at( appName ), SIGNAL( finished ( int , QProcess::ExitStatus ) ),
            this, SLOT( app_closed( int , QProcess::ExitStatus ) ) );
        }
        else
        {
          std::cout << "Error: App not start in time!" << std::endl;
        }
      }
      else
      {
        std::cerr << "Error: App '" << appName << "' not found!" << std::endl;
      }
    }
  }

  void MainWindow::addAppToMap( const std::string& appName, const std::string& appPath )
  {
    Application* newApp = new Application( appName, appPath );
    _loadedApps[ appName ] = newApp;
  }

  void MainWindow::addGroupToGrid( QString qKey )
  {

    std::string key = qKey.toStdString( );
    auto syncGroup = _syncGroups[ key ];

    //Add group to widgets groups
    QLabel* nameLabel = new QLabel( QString::fromStdString( syncGroup->getName( ) ) );
    QLabel* ownerLabel = new QLabel( QString::fromStdString( syncGroup->getOwner( ) ) );
    QPushButton* closePushButton = new QPushButton( );
    closePushButton->setIcon( QIcon( ":/icons/iconClose.png" ) );
    _widgetsGroups[ key ] = new WidgetsGroup( nameLabel, ownerLabel, closePushButton );

    //Add group to grid layout
    ui->groupsGridLayout->addWidget( nameLabel );
    ui->groupsGridLayout->addWidget( ownerLabel );
    ui->groupsGridLayout->addWidget( closePushButton );

    //Add close push button event
    connect( closePushButton, SIGNAL( clicked( bool ) ), this, SLOT( closeGroup_clicked( ) ) );
  }

  void MainWindow::closeGroup_clicked( )
  {

    // Checks for XML loaded
    if ( ui->xmlFilename->text( ) == QString( "" ) )
    {
      std::cout << "Error closing group: XML file is not loaded." << std::endl;
      return;
    }

    auto closeButton = qobject_cast< QPushButton* >( sender( ) );

    std::map<std::string, WidgetsGroup*>::iterator it;
    std::string key;
    for ( it = _widgetsGroups.begin( ); it != _widgetsGroups.end( ); ++it )
    {
      if ( it->second->getClosePushButton() == closeButton )
      {
        key = it->first;
        break;
      }
    }

    // Checks for app closed
    auto syncGroup = _syncGroups[ key ];

    std::string owner = syncGroup->getOwner( );
    QPushButton* appPushButton = ui->groupBox->findChild< QPushButton* >( QString::fromStdString( owner ) );
    if ( !appPushButton->isEnabled( ) )
    {
      std::cout << "Error closing group: " << appPushButton->text( ).toStdString( ) << " is not closed." << std::endl;
      return;
    }

    // If XML loaded && owner app closed
    removeRowGroup( key, true ); // true asks to confirm to remove row group

  }

  void MainWindow::removeRowGroup( const std::string& key, const bool& askFirst )
  {

    if ( askFirst )
    {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, "Remove group", "Do you want to remove '" +
        QString::fromStdString( key ) + "'' group?", QMessageBox::Yes|QMessageBox::No);
      if ( reply != QMessageBox::Yes )
      {
          return;
      }
    }

    auto widget = _widgetsGroups[ key ]->getNameLabel( );

    int index = ui->groupsGridLayout->indexOf( widget );
    auto gp = qMakePair( -1, -1 );
    int rs, cs;
    ui->groupsGridLayout->getItemPosition( index, &gp.first, &gp.second, &rs, &cs );
    unsigned int row = gp.first;

    // Remove row from grid
    Auxiliars::removeRow( ui->groupsGridLayout, row );

    // Remove group from widgets
    _widgetsGroups.erase( key );

    // Remove group from map
    _syncGroups.erase ( key );

    //Send publish destroy group event
    manco::ZeqManager::instance( ).publishDestroyGroup( key );

    std::cout << "Group: " << key << " removed successfully." << std::endl;

  }

  void MainWindow::changeGroupName( QString qKey, QString qNewName )
  {

    std::string key = qKey.toStdString( );
    std::string newName = qNewName.toStdString( );

    //Sync groups
    auto syncGroup = _syncGroups[ key ];
    std::string name = syncGroup->getName( );
    syncGroup->setName( newName );

    //Widgets groups
    WidgetsGroup* widgetsGroup = _widgetsGroups[ key ];
    QLabel* nameLabel = widgetsGroup->getNameLabel( );
    nameLabel->setText( qNewName );

    std::cout << "Group: " << key << ". Changed group name '" << name <<
      "'' to '" << newName << "' succesfully." << std::endl;

  }

}

