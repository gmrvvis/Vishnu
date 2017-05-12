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
#define DEFAULT_ZEQ_SESSION "hbp://"

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
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

  protected:
    void closeEvent( QCloseEvent *event ) override;

  private slots:
    void buttonLoadXml_clicked( );
    void pushButtonApp_clicked( );
    void app_closed( int exitCode, QProcess::ExitStatus exitStatus );
    void checkApps( );
    void closeGroup_clicked( );
    void addGroup( const QString& qKey );
    void changeGroupName( const QString& qKey, const QString& qNewName );
    void removeGroup( const QString& qKey, const bool& askFirst = false );

  private:
    vishnu::Ui::MainWindow *_ui;
    std::string _zeqSession;
    std::map<std::string, Application*> _apps; //appName, Application
    std::map<std::string, SyncGroup*> _syncGroups; //key, SyncGroup
    std::map<std::string, WidgetsGroup*> _widgetsGroups; //key, WidgetsGroup

    void addApp( const std::string& appName, const std::string& appPath );
    void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
    void receivedChangeNameGroupUpdate( zeroeq::gmrv::ConstChangeNameGroupPtr o );
    void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o );

  signals:
    void signalCreateGroup( const QString& );
    void signalChangeGroupName( const QString&, const QString& );
    void signalRemoveGroup( const QString&, const bool& );

  };

}
#endif // MAINWINDOW_H

