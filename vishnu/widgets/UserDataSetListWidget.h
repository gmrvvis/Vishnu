/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_USERDATASETLISTWIDGET_H
#define VISHNU_USERDATASETLISTWIDGET_H

#include <string>
#include <memory>

#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

#include <sp1common/sp1common.h>

#include "UserDataSetWidget.h"
#include "../model/UserDataSets.h"

namespace vishnu
{

  class UserDataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit UserDataSetListWidget( QWidget* parent = Q_NULLPTR );

      UserDataSetWidgetPtr addUserDataSet( const std::string& name,
        const std::string& path, const std::string& csvFilename,
        const std::string& jsonFilename, const std::string& xmlFilename,
        const bool& selected );

      void removeCurrentDataSet( void );
      void selectCurrentDataSet( void );
      std::string getCurrentDataSetName( void );
      UserDataSetPtr getCurrentDataSet( );
      UserDataSetMap getDataSets( void );
      void clearDataSets( void );
      void setBlurred( const bool& state );

    protected:

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

    signals:

      void addUserDataSetEvent( const std::string& filePath );

    //private slots:

      //void slotCheckSelectedDataSets( bool checked );

    private:

      bool _checkingProperty;

  };

}

#endif
