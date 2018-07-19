#ifndef VISHNU_MAINWINDOW_H
#define VISHNU_MAINWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include <QMutex>

#include <QStatusBar>
#include <QMenuBar>
#include <QWidget>
#include <QToolBar>
#include <QListWidgetItem>
#include <QDir>

#include "Application.h"
#include "widgets/DataSetListWidget.h"
#include "widgets/PropertiesTableWidget.h"
#include "widgets/ZEQGroupListWidget.h"

#include <manco/manco.h>
#include <sp1common/sp1common.h>

typedef std::vector< std::string > StringVector;
Q_DECLARE_METATYPE( StringVector );

namespace vishnu
{
  typedef std::shared_ptr< Application > ApplicationPtr;
  typedef std::map< std::string, ApplicationPtr > Applications;

  typedef std::shared_ptr< DataSetListWidget > DataSetListWidgetPtr;
  typedef std::shared_ptr< PropertiesTableWidget > PropertiesTableWidgetPtr;
  typedef std::shared_ptr< ZEQGroupListWidget > ZEQGroupListWidgetPtr;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      explicit MainWindow(sp1common::Args args,
        QWidget *parent = 0 );
      ~MainWindow();

    protected:
      void closeEvent( QCloseEvent *event ) override;

    private slots:
      void runApp( );
      void closeApp( int exitCode, QProcess::ExitStatus exitStatus );
      void loadClint();
      void loadDCExplorer();
      void loadPyramidal();
      void checkApps( );
      bool generateDataFiles( QDir dir );

      void addDataSetItem( const std::string& dropped = std::string( ) );
      void removeDataSetItem( );

      void syncGroup( const QString& key, const QString& name,
       const QString& owner, const StringVector& ids,
       const QColor& color );
      void changeGroupName( const QString& key, const QString& name );
      void changeGroupColor( const QString& key, const QColor& color );
      void removeSelectedGroup( );
      void removeGroup( const QString& key );

  private:

      //vishnu::Ui::MainWindow *_ui;
      std::string _zeqSession;
      std::string _workingDirectory;
      bool _closingProcesses;
      Applications _applications;

      QAction* addDataSetAction;
      QAction* removeDataSetAction;
      QAction* removeGroupAction;

      DataSetListWidgetPtr _dataSetListWidget;
      PropertiesTableWidgetPtr _propertiesTableWidget;
      ZEQGroupListWidgetPtr _zeqGroupListWidget;

      void initZeqSession( );
      void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
      void receivedChangeNameGroup( zeroeq::gmrv::ConstChangeNameGroupPtr o );
      void receivedChangeColorGroup( zeroeq::gmrv::ConstChangeColorGroupPtr o );
      void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o );

    signals:
      void signalSyncGroup( const QString&, const QString&, const QString&,
        const StringVector&, const QColor& );
      void signalChangeGroupName( const QString&, const QString& );
      void signalChangeGroupColor( const QString&, const QColor& );
      void signalDestroyGroup( const QString& );
  };

}

#endif

