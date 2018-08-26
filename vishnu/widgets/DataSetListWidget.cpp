/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#include "DataSetListWidget.h"

#include <algorithm>

#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{
  DataSetListWidget::DataSetListWidget( QWidget* parent )
      : QListWidget( parent )
  {    

    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::InternalMove );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );

    setMouseTracking( true );
    setStyleSheet(
      "QListWidget::item:selected{border: 1px solid #6a6a6a;background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 0.5 #FFFFFF, stop: 1 #AAAAFF );}"
      "QListWidget::item:!selected{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #CCCCFF, stop: 0.5 #FFFFFF, stop: 1.0 #CCCCFF );}"
      "QListWidget::item:hover{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #BBBBFF, stop: 0.5 #FFFFFF, stop: 1 #BBBBFF );}"
    );

     _propertyGroups.reset( new sp1common::PropertyGroups( ) );
  }

  void DataSetListWidget::createDataSetsFromSEG(
    DataSetWidgets& /*dataSetWidgets*/, const std::string& /*path*/ )
  {

  }

  void DataSetListWidget::createDataSetsFromJSON(
    DataSetWidgets& dataSetWidgets, const std::string& path )
  {
    sp1common::DataSetsPtr dataSets =
      sp1common::JSON::deserialize< sp1common::DataSets >( path );

    for ( const auto& dataSet : dataSets->getDataSets( ) )
    {
      createDataSetFromCSV( dataSetWidgets, dataSet->getPath( ),
        dataSet->getProperties( ) );
    }

    _propertyGroups = dataSets->getPropertyGroups( );

  }

  void DataSetListWidget::createDataSetFromCSV( DataSetWidgets& dataSetWidgets,
     const std::string& path, const sp1common::Properties& properties )
  {
    bool notUsedPath = true;
    for ( int i = 0; i < count( ); ++i )
    {
      DataSetWidgetPtr dataSetWidget = static_cast< DataSetWidgetPtr >(
        itemWidget( item( i ) ) );
      if ( dataSetWidget->getPath( ) == path )
      {
        QMessageBox::warning( this, APPLICATION_NAME,
          tr( "This file is already loaded." ) );
        notUsedPath = false;
        break;
      }
    }

    if ( notUsedPath )
    {
      sp1common::DataSetPtr dataSet( new sp1common::DataSet( path ) );

      if ( properties.size( ) == 0 ) //CSV
      {
        std::vector< std::string > headers =
          sp1common::Files::readCsvHeaders( path );

        for ( const auto& header : headers )
        {
          dataSet->addProperty( sp1common::PropertyPtr(
            new sp1common::Property( header, sp1common::DataCategory::Undefined,
            sp1common::DataStructureType::None ) ) );
        }
      }
      else //JSON or SEG
      {
        dataSet->setProperties( properties );
      }

      //Add to dataset
      DataSetWidgetPtr dataSetWidget( new DataSetWidget( dataSet ) );
      dataSetWidget->setListWidgetItem( new QListWidgetItem( this ) );

      QListWidgetItem* listWidgetItem = dataSetWidget->getListWidgetItem( );
      addItem( listWidgetItem );
      listWidgetItem->setSizeHint( dataSetWidget->sizeHint ( ) );
      setItemWidget( listWidgetItem, dataSetWidget );

      dataSetWidgets.emplace_back( dataSetWidget );
    }
  }

  DataSetWidgets DataSetListWidget::addDataSets(
    const std::vector< std::string >& dropped )
  {
    DataSetWidgets dataSetWidgets;

    QStringList filePaths;

    if ( !dropped.empty( ) )
    {
      for (unsigned int i = 0; i < dropped.size( ); ++i )
      {
        filePaths << QString::fromStdString( dropped.at( i ) );
      }
    }
    else
    {
      filePaths = QFileDialog::getOpenFileNames( this,
        QString( "Select files" ), QString( "" ),
        QString( "CSV-Files(*.csv);;JSON-Files(*.json);;SEG-Files(*.seg)" ) );
    }
    if ( filePaths.isEmpty( ) )
    {
      return dataSetWidgets;
    }

    filePaths.removeDuplicates( );

    for ( int i = 0; i < filePaths.count( ); ++i )
    {
      QString qFilePath = filePaths.at( i );
      std::string filepath = qFilePath.toStdString( );

      std::string extension = sp1common::Strings::lower(
        QFileInfo( qFilePath ).completeSuffix( ).toStdString( ) );

      if ( extension == STR_EXT_SEG )
      {
        createDataSetsFromSEG( dataSetWidgets, filepath );
      }
      else if ( extension == STR_EXT_JSON )
      {
        createDataSetsFromJSON( dataSetWidgets, filepath );
      }
      else if ( extension == STR_EXT_CSV )
      {
        createDataSetFromCSV( dataSetWidgets, filepath );
      }
    }

    return dataSetWidgets;
  }

  std::vector< std::string > DataSetListWidget::getPropertiesToRemove( )
  {
    DataSetWidgetPtr currentDsw = static_cast< DataSetWidgetPtr >(
      itemWidget( currentItem( ) ) );

    std::vector< std::string > propertiesToRemove =
      currentDsw->getDataSet( )->getPropertyNames( );

    for( int row = 0; row < count( ); ++row )
    {
      QListWidgetItem* listItem = item( row );
      if ( listItem != currentItem( ) )
      {
        DataSetWidgetPtr dsw = static_cast< DataSetWidgetPtr >(
          itemWidget( listItem ) );
        std::vector< std::string > properties =
          dsw->getDataSet( )->getPropertyNames( );
        for ( const auto& property: properties )
        {
          propertiesToRemove.erase( std::remove( propertiesToRemove.begin( ),
            propertiesToRemove.end( ), property ), propertiesToRemove.end( ) );
        }
      }
    }

    return propertiesToRemove;
  }

  void DataSetListWidget::removeCurrentDataSet( )
  {    
    takeItem( row( currentItem( ) ) );
  }

  sp1common::DataSetsPtr DataSetListWidget::getDataSets( void ) const
  {
    sp1common::DataSetsPtr dataSets( new sp1common::DataSets( ) );

    for( int row = 0; row < count( ); ++row )
    {
      DataSetWidgetPtr dsw = static_cast< DataSetWidgetPtr >(
        itemWidget( item( row ) ) );

      sp1common::DataSetPtr dataSet( new sp1common::DataSet( dsw->getPath( ) ) );
      dataSet->setProperties( dsw->getDataSet( )->getProperties( ) );
      dataSets->addDataSet( dataSet );
    }

    return dataSets;
  }

  sp1common::PropertyGroupsPtr DataSetListWidget::getPropertyGroups(
    void ) const
  {
    return _propertyGroups;
  }

  void DataSetListWidget::setPropertyGroups(
    const sp1common::PropertyGroupsPtr& propertyGroups )
  {
    _propertyGroups = propertyGroups;
  }

  std::vector< std::string > DataSetListWidget::getCommonProperties( )
  {
    std::vector< std::string > commonProperties;
    for ( const auto& dataset : getDataSets( )->getDataSets( ) )
    {
      std::vector< std::string > dataSetHeaders = dataset->getPropertyNames( );

      if ( !commonProperties.empty( ) )
      {
        commonProperties = sp1common::Vectors::intersect( commonProperties,
          dataSetHeaders );
      }
      else
      {
        commonProperties = dataSetHeaders;
      }
    }

    return commonProperties;
  }

  void DataSetListWidget::dragEnterEvent( QDragEnterEvent* event )
  {
    if ( event->mimeData()->hasUrls( ) )
    {
      event->acceptProposedAction();
    }
  }

  void DataSetListWidget::dragMoveEvent( QDragMoveEvent* event )
  {
    event->acceptProposedAction();
  }

  void DataSetListWidget::dragLeaveEvent( QDragLeaveEvent* event )
  {
    event->accept();
  }

  void DataSetListWidget::dropEvent( QDropEvent* event )
  {
    std::vector< std::string > files;
    for ( const QUrl& url : event->mimeData( )->urls( ) )
    {
      QString filePath = url.toLocalFile();

      std::string extension = sp1common::Strings::lower(
        QFileInfo( filePath ).completeSuffix( ).toStdString( ) );

      if ( sp1common::Vectors::find( { STR_EXT_CSV, STR_EXT_JSON, STR_EXT_SEG },
        extension ) != -1 )
      {
        files.emplace_back( filePath.toStdString( ) );
      }
    }
    if ( !files.empty( ) )
    {
      sp1common::Debug::consoleMessage( "Dropped files" );
      emit signalAddFiles( files );
    }
  }
}
