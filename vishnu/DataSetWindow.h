/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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
