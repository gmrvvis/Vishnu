#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include "Application.h"
#include "SyncGroup.h"

#define APICOLAT "APICOLAT"
#define CLINT "CLINT"
#define SPINERET "SPINERET"

#define DELIMITER "|&|"

namespace vishnu
{
  namespace Ui
  {
    class MainWindow;
  }

  struct AppProperty
  {
      std::string launcherCmd;
      QPushButton* pushButton;
  };

  class MainWindow : public QMainWindow
  {
      Q_OBJECT

  public:
      explicit MainWindow( QWidget *parent = 0 );
      ~MainWindow();


  protected:
      void closeEvent( QCloseEvent *event ) override;


  private slots:
      void buttonLoadXml_clicked( );
      void pushButtonApp_clicked( );
      void app_closed( int exitCode, QProcess::ExitStatus exitStatus );
      void checkAppsOK( );
      void closeGroup_clicked( );
      void addGroupToGrid( QString groupName, QString owner );

  private:
      vishnu::Ui::MainWindow *ui;
      std::map< std::string, Application* > _loadedApps;
      void addAppToMap( std::string appName, std::string appPath );

      void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
      std::vector<SyncGroup*> _syncGroups;

  signals:
    void signalCreateGroup( QString, QString );
  };

}
#endif // MAINWINDOW_H

