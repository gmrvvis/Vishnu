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

#ifndef VISHNU_ZEQGROUPLISTWIDGET_H
#define VISHNU_ZEQGROUPLISTWIDGET_H

#include <string>
#include <vector>

#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

#include <vishnucommon/vishnucommon.h>

#include "ZEQGroupWidget.h"

namespace vishnu
{

  class ZEQGroupListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit ZEQGroupListWidget( QWidget* parent = Q_NULLPTR );

      ZEQGroupWidgetPtr syncGroup( const std::string& key, const QString& name,
        const QString& owner, const std::vector< std::string >& ids,
        const QColor& color );

      void changeGroupName( const std::string& key, const QString& name );

      void changeGroupColor( const std::string& key, const QColor& color );

      void removeCurrentGroup( );

      void removeGroup( const std::string& key );

      std::string getKey( QListWidgetItem* listWidgetItem );

      void setBlurred( const bool& state );
    private:

      ZEQGroupWidgetPtr findGroup( const std::string& key );

    signals:

      void addDataSetEvent( const std::string& filePath );

  };

}

#endif
