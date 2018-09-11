/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_DATASETLISTWIDGET_H
#define VISHNU_DATASETLISTWIDGET_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

#include <sp1common/sp1common.h>

#include "DataSetWidget.h"

namespace vishnu
{

  class DataSetListWidget;
  typedef std::shared_ptr< DataSetListWidget > DataSetListWidgetPtr;

  class DataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit DataSetListWidget( QWidget* parent = Q_NULLPTR );

      DataSetWidgets addDataSets( const std::vector< std::string >& dropped =
        std::vector< std::string >( ) );

      std::vector< std::string > getPropertiesToRemove( void );

      void removeCurrentDataSet( void );

      std::vector< std::string > getCommonProperties( void );

      sp1common::DataSetsPtr getDataSets( void ) const;

      sp1common::PropertyGroupsPtr getPropertyGroups( void ) const;

      void setPropertyGroups(
        const sp1common::PropertyGroupsPtr& propertyGroups );

    protected:

      void dragEnterEvent( QDragEnterEvent* event );

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

      void dropEvent( QDropEvent* event );

    signals:

      void signalAddFiles( const std::vector< std::string >& files );

    private:

      void createDataSetsFromSEG( DataSetWidgets& dataSetWidgets,
        const std::string& path );

      void createDataSetsFromJSON( DataSetWidgets& dataSetWidgets,
        const std::string& path );

      void createDataSetFromCSV( DataSetWidgets& dataSetWidgets,
        const std::string& path,
        const sp1common::Properties& properties = sp1common::Properties( ) );

      sp1common::PropertyGroupsPtr _propertyGroups;

  };

}

#endif
