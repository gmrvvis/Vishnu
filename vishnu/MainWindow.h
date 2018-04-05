#ifndef VISHNU_MAINWINDOW_H
#define VISHNU_MAINWINDOW_H

#include <string>
#include <map>
#include <memory>

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include <QMutex>

#include <QToolBar>
#include <QListWidgetItem>

#include "Application.h"
#include "SyncGroup.h"
#include "WidgetsGroup.h"

#include <manco/manco.h>
#include <sp1common/sp1common.h>

namespace vishnu
{
  namespace Ui
  {
    class MainWindow;
  }

  typedef std::shared_ptr< Application > ApplicationPtr;
  typedef std::map< std::string, ApplicationPtr > Applications;

  typedef std::shared_ptr< SyncGroup > SyncGroupPtr;
  typedef std::map< std::string, SyncGroupPtr > SyncGroups;

  typedef std::shared_ptr< WidgetsGroup > WidgetsGroupPtr;
  typedef std::map< std::string, WidgetsGroupPtr > WidgetsGroups;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      explicit MainWindow(sp1common::Args args,
        QWidget *parent = 0 );
      ~MainWindow();

    public slots:


    protected:
      void closeEvent( QCloseEvent *event ) override;

    private slots:
      void pushButtonApp_clicked( );
      void app_closed( int exitCode, QProcess::ExitStatus exitStatus );
      void loadClint();
      void loadDCExplorer();
      void loadPyramidal();
      void checkApps( );
      void removeGroup_clicked( );
      void createGroup( const QString& qKey );
      void changeGroupName( const QString& qKey, const QString& qNewName );
      void destroyGroup(const QString& qKey);

      void addDataSetItem();
      void removeDataSetItem( );

      void itemClicked( QListWidgetItem *item );

      void cellSelected( int nRow, int nCol );

  private:

      vishnu::Ui::MainWindow *_ui;
      std::string _zeqSession;
      std::string _workingDirectory;
      bool _closingProcesses;
      Applications _applications;
      SyncGroups _syncGroups;
      WidgetsGroups _widgetsGroups;

      QAction *addDataSetAction;
      QAction *removeDataSetAction;

      void initZeqSession( );
      void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
      void receivedChangeNameGroupUpdate(
        zeroeq::gmrv::ConstChangeNameGroupPtr o );
      void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o );

    signals:
      void signalCreateGroup( const QString& );
      void signalChangeGroupName( const QString&, const QString& );
      void signalDestroyGroup( const QString& );
  };

}

#endif

