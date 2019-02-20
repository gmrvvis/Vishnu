/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
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

#include <vishnucommon/vishnucommon.h>

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
