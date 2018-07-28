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
    setDragDropMode( QAbstractItemView::DragDrop );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );
  }


  void DataSetListWidget::createDataSetFromCSV( const std::string& path,
    DataSetWidgets& dataSetWidgets )
  {
    bool notUsedPath = true;
    for ( int i = 0; i < count( ); ++i )
    {
      DataSetWidget* dataSetWidget = static_cast< DataSetWidget* >(
        itemWidget( item( i ) ) );
      if ( dataSetWidget->getPath( ) == path )
      {
        QMessageBox::warning( this, APPLICATION_NAME,
          tr("This file is already loaded.") );
        notUsedPath = false;
        break;
      }
    }

    if ( notUsedPath )
    {
      //Add to dataset
      DataSetWidgetPtr dataSetWidget( new DataSetWidget( path ) );
      dataSetWidget->setListWidgetItem( new QListWidgetItem( this ) );

      QListWidgetItem* listWidgetItem = dataSetWidget->getListWidgetItem( );
      addItem( listWidgetItem );
      listWidgetItem->setSizeHint( dataSetWidget->sizeHint ( ) );
      setItemWidget( listWidgetItem, dataSetWidget );

      dataSetWidgets.push_back( dataSetWidget );
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
        QString( "Choose CSV file" ), QString( "" ),
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

      }
      else if ( extension == STR_EXT_JSON )
      {

      }
      else if ( extension == STR_EXT_CSV )
      {
        createDataSetFromCSV( filepath, dataSetWidgets );
      }
    }

    return dataSetWidgets;
  }

  std::vector< std::string > DataSetListWidget::getPropertiesToRemove( )
  {
    DataSetWidget* currentDsw = static_cast<DataSetWidget*>(
      itemWidget( currentItem( ) ) );

    std::vector< std::string > propertiesToRemove = currentDsw->getHeaders( );

    for( int row = 0; row < count( ); ++row )
    {
      QListWidgetItem* listItem = item( row );
      if ( listItem != currentItem( ) )
      {
        DataSetWidget* dsw = static_cast< DataSetWidget* >(
          itemWidget( listItem ) );
        std::vector< std::string > properties = dsw->getHeaders( );
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

  DataSets DataSetListWidget::getDataSets( )
  {
    DataSets dataSets;

    for( int row = 0; row < count( ); ++row )
    {
      DataSetWidget* dsw = static_cast< DataSetWidget* >(
        itemWidget( item( row ) ) );

      DataSetPtr dataSet( new DataSet( dsw->getPath( ), dsw->getHeaders( ) ) );
      dataSets[ dsw->getPath() ] = dataSet;
    }

    return dataSets;
  }

  std::vector< std::string > DataSetListWidget::getCommonProperties( )
  {
    std::vector< std::string > commonProperties;
    for ( const auto& dataset : getDataSets( ) )
    {
      std::vector< std::string > dataSetHeaders = dataset.second->getHeaders( );

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
