#ifndef VISHNU_MAINWINDOW_H
#define VISHNU_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include <QMutex>
#include "Application.hpp"
#include "SyncGroup.hpp"
#include "WidgetsGroup.hpp"

#include <manco/manco.h>
#include <sp1common/sp1common.h>

namespace vishnu
{
  namespace Ui
  {
    class MainWindow;
  }

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
      void buttonLoadCsv_clicked( );
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

    private:
      vishnu::Ui::MainWindow *_ui;
      std::string _zeqSession;
      std::string _workingDirectory;
      bool _closingProcesses;
      std::map<manco::ApplicationType, Application*> _apps; //appName, Application
      std::map<std::string, SyncGroup*> _syncGroups; //key, SyncGroup
      std::map<std::string, WidgetsGroup*> _widgetsGroups; //key, WidgetsGroup
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

