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

#ifndef VISHNU_DATASETWINDOW_H
#define VISHNU_DATASETWINDOW_H

#include <QDialog>
#include <QAction>
#include <QToolBar>
#include <QPushButton>
#include <QDir>

#include <string>
#include <vector>

#include "widgets/PathsWidget.h"
#include "widgets/DataSetListWidget.h"
#include "widgets/PropertiesTableWidget.h"

Q_DECLARE_METATYPE( std::vector< std::string > )

namespace vishnu
{

  class DataSetWindow : public QDialog
  {
      Q_OBJECT

      public:
        explicit DataSetWindow( QWidget *parent = Q_NULLPTR );
        ~DataSetWindow();
        UserDataSetPtr getResultUserDataSet( void );

      private slots:
        void slotCreateButton( void );
        void slotCancelButton( void );
        void slotAddFiles( const std::vector< std::string >& dropped =
          std::vector< std::string >( ) );
        void slotRemoveDataSet( );

      private:
        QToolBar* _toolBar;
        PathsWidgetPtr _pathsWidget;
        DataSetListWidgetPtr _dataSetListWidget;
        PropertiesTableWidgetPtr _propertiesTableWidget;
        QPushButton* _cancelButton;
        QPushButton* _createButton;

        bool createCSV( const std::string& csvPath,
          const vishnucommon::PropertyGroupsPtr& propertyGroups );
        bool createXML( const std::string& path, const std::string& csvPath,
          const std::string& xmlPath,
          const vishnucommon::DataSetPtr& resultDataSet,
          const vishnucommon::PropertyGroupsPtr& propertyGroups );
        bool createJSON( const std::string& jsonPath,
          vishnucommon::DataSetsPtr& dataSets );
        bool createGeometricData( const std::string& path );

  };

}

#endif
