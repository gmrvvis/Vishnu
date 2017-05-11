#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include "Application.hpp"
#include "SyncGroup.hpp"
#include "WidgetsGroup.hpp"

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
    void addGroupToGrid( QString key );
        void changeGroupName(QString qKey, QString qNewName );

  private:
    vishnu::Ui::MainWindow *ui;
    std::map< std::string, Application* > _loadedApps;
    std::map<std::string, SyncGroup*> _syncGroups;
    std::map<std::string, WidgetsGroup*> _widgetsGroups;

    void addAppToMap( const std::string& appName, const std::string& appPath );
    void removeRowGroup( const std::string& key, const bool& askFirst = false );

    void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
    void receivedChangeNameGroupUpdate( zeroeq::gmrv::ConstChangeNameGroupPtr o);
    void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o);

  signals:
    void signalCreateGroup( QString );
    void signalChangeGroupName( QString, QString );
  };

}
#endif // MAINWINDOW_H

