/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
#include <vishnucommon/vishnucommon.h>

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
      explicit MainWindow( vishnucommon::Args args,
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
      void receivedSyncGroup( vishnulex::ConstSyncGroupPtr o );
      void receivedChangeNameGroup( vishnulex::ConstChangeNameGroupPtr o );
      void receivedChangeColorGroup( vishnulex::ConstChangeColorGroupPtr o );
      void receivedDestroyGroup( vishnulex::ConstDestroyGroupPtr o );

      void loadApps( void );
      void setBlurred( const bool& state );

      void addUserDataSet( const UserDataSetPtr& userDataSet );
      void removeUserDataSet( const UserDataSetPtr& userDataSet );
      UserDataSetsPtr readUserDataSets( void );
      void writeUserDataSets( const UserDataSetsPtr& userDataSets );

  };

}

#endif
