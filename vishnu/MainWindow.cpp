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

namespace vishnu
{

  MainWindow::MainWindow( QWidget *parent )
      : QMainWindow( parent )
      , ui( new Ui::MainWindow )
  {
      ui->setupUi( this );     

      connect( ui->buttonLoadXml, SIGNAL( clicked( bool ) ), this, SLOT( buttonLoadXml_clicked( ) ) );

      addAppToMap( APICOLAT, "gnome-terminal" );
      addAppToMap( CLINT, "/home/gbayo/dev/ClintExplorer/build/bin/ClintExplorer" );
      addAppToMap( SPINERET, "/home/gbayo/dev/spineret/build/bin/CellExplorer" );

      for ( const auto& app : _loadedApps )
      {
          ui->verticalLayout_2->addWidget( app.second->getPushButton( ) );
          connect( app.second->getPushButton( ), SIGNAL( clicked( bool ) ), this, SLOT( pushButtonApp_clicked( ) ) );
      }

      setMaximumHeight(height());

      //ui->gridLayout_3->setColumnStretch(2, 20);
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
        arguments << "-x" << ui->xmlFilename->text();

        appButton->setEnabled( false );

        _loadedApps.at( appName )->setReadChannel( QProcess::StandardOutput );
        _loadedApps.at( appName )->waitForReadyRead( );
        _loadedApps.at( appName )->readAllStandardOutput( );
        _loadedApps.at( appName )->start( program, arguments );

        if (_loadedApps.at( appName )->waitForStarted(500))
        {
          QByteArray out_data = _loadedApps.at( appName )->readAllStandardOutput( );
          QString out_string( out_data );
          std::cout << out_string.toStdString().c_str( ) << std::endl;
          connect( _loadedApps.at( appName ), SIGNAL( finished ( int , QProcess::ExitStatus ) ),
            this, SLOT( app_closed( int , QProcess::ExitStatus ) ) );

            //addGroupToGrid( "testName", appName ); //TESTING
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

  void MainWindow::addAppToMap( std::string appName, std::string appPath )
  {
    Application* newApp = new Application( appName, appPath );
    _loadedApps[ appName ] = newApp;
  }

  void MainWindow::addGroupToGrid( std::string groupName, std::string owner )
  {
    QLabel* groupNameLabel = new QLabel( QString::fromStdString( groupName ) );
    QLabel* ownerNameLabel = new QLabel( QString::fromStdString( owner ) );
    QPushButton* closeButton = new QPushButton(/*QString( "Close" )*/);

    QPixmap pixmap( QString("/home/gbayo/dev/vishnu/vishnu/iconClose.png") );
    QIcon icon( pixmap );
    closeButton->setIcon( icon );
    closeButton->setIconSize( QSize ( 24, 24 ) );
   // closeButton->setIconSize( pixmap.rect( ).size( ) );
    //closeButton->setFixedSize( pixmap.rect( ).size( ) );

    ui->gridLayout_3->addWidget( groupNameLabel );
    ui->gridLayout_3->addWidget( ownerNameLabel );
    ui->gridLayout_3->addWidget( closeButton );

    connect( closeButton, SIGNAL( clicked( bool ) ), this, SLOT( closeGroup_clicked( ) ) );
  }

  void MainWindow::closeGroup_clicked( )
  {

    QPushButton* closeButton = qobject_cast< QPushButton* >( sender( ) );
    int closeButtonIndex = ui->gridLayout_3->indexOf( closeButton );
    auto closeButtonPosition = qMakePair( -1, -1 );
    int closeButtonRowSpan, closeButtonColumnSpan;
    ui->gridLayout_3->getItemPosition( closeButtonIndex, &closeButtonPosition.first,
      &closeButtonPosition.second, &closeButtonRowSpan, &closeButtonColumnSpan );

    for( int columnIndex = 0; columnIndex < ui->gridLayout_3->columnCount( ); ++columnIndex )
    {
      QLayoutItem* const item = ui->gridLayout_3->itemAtPosition( closeButtonPosition.first, columnIndex );
      item->widget( )->deleteLater( );
    }
    //ui->gridLayout_3->setRowMinimumHeight(closeButtonPosition.first, 0);
    //ui->gridLayout_3->setRowStretch(closeButtonPosition.first, 0);

  }

}

