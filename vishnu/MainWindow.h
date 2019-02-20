/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

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

#include "AppProcess.h"
#include "widgets/UserDataSetListWidget.h"
#include "widgets/ZEQGroupListWidget.h"
#include "model/UserPreferences.h"

#include <manco/manco.h>
#include <sp1common/sp1common.h>

namespace vishnu
{
  using AppProcessPtr = std::shared_ptr< AppProcess >;
  using AppProcessMap = std::map< std::string, AppProcessPtr >;
  using UserDataSetListWidgetPtr = std::shared_ptr< UserDataSetListWidget >;
  using ZEQGroupListWidgetPtr = std::shared_ptr< ZEQGroupListWidget >;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      explicit MainWindow( sp1common::Args args,
        QWidget *parent = 0 );
      ~MainWindow();

    protected:
      void closeEvent( QCloseEvent *event ) override;

    private slots:

      void slotRemoveUserDataSet( void );
      void slotCheckUserDataSet( bool checke );
      void slotReloadDataSets( void );
      void runApp( void );
      void closeApp( int exitCode, QProcess::ExitStatus exitStatus );

      void checkApps( bool checked );
      void addDataSet( void );

      void syncGroup( const QString& key, const QString& name,
       const QString& owner, const std::vector< std::string >& ids,
       const QColor& color );
      void changeGroupName( const QString& key, const QString& name );
      void changeGroupColor( const QString& key, const QColor& color );
      void removeSelectedGroup( );
      void removeGroup( const QString& key );

  signals:
    void signalSyncGroup( const QString&, const QString&, const QString&,
      const std::vector< std::string >&, const QColor& );
    void signalChangeGroupName( const QString&, const QString& );
    void signalChangeGroupColor( const QString&, const QColor& );
    void signalDestroyGroup( const QString& );
    void signalReloadDataSets( );

  private:

      UserPreferencesPtr _userPreferences;
      bool _closingProcesses;
      AppProcessMap _applications;

      QAction* removeGroupAction;

      UserDataSetListWidgetPtr _userDataSetListWidget;
      ZEQGroupListWidgetPtr _zeqGroupListWidget;

      void initZeqSession( );
      void receivedSyncGroup( zeroeq::gmrv::ConstSyncGroupPtr o );
      void receivedChangeNameGroup( zeroeq::gmrv::ConstChangeNameGroupPtr o );
      void receivedChangeColorGroup( zeroeq::gmrv::ConstChangeColorGroupPtr o );
      void receivedDestroyGroup( zeroeq::gmrv::ConstDestroyGroupPtr o );

      void loadApps( void );
      void setBlurred( const bool& state );

      void addUserDataSet( const UserDataSetPtr& userDataSet );
      void removeUserDataSet( const UserDataSetPtr& userDataSet );
      UserDataSetsPtr readUserDataSets( void );
      void writeUserDataSets( const UserDataSetsPtr& userDataSets );

  };

}

#endif
