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

#ifndef VISHNU_DATASETWIDGET_H
#define VISHNU_DATASETWIDGET_H

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <string>
#include <vector>

#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class DataSetWidget;
  using DataSetWidgetPtr = DataSetWidget*;
  using DataSetWidgets = std::vector< DataSetWidgetPtr >;

  class DataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit DataSetWidget( const vishnucommon::DataSetPtr& dataSet,
      QWidget* parent = Q_NULLPTR );

    std::string getPath( void ) const;
    void setPath( const std::string& path );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

    vishnucommon::DataSetPtr getDataSet( void ) const;

  signals:

    void removeSelected( void );
    void updatePath( void );

  private slots:

    void clickPath( void );
    void clickRemove( void );

  private:

    QLabel* _path = nullptr;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;
    vishnucommon::DataSetPtr _dataSet;

  };

}

#endif
