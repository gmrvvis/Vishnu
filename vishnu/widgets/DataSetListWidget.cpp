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
  DataSetListWidget::DataSetListWidget( QWidget* /*parent*/ )
  {    
    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::DragDrop );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );
  }

  DataSetWidget* DataSetListWidget::addDataSet( const std::string& dropped )
  {
    QString filePath;

    if ( !dropped.empty( ) )
    {
      filePath = QString::fromStdString( dropped );
    }
    else
    {
      filePath = QFileDialog::getOpenFileName( this,
        QString( "Choose CSV file" ), QString( "" ),
        QString( "CSV-Files(*.csv)" ) );
    }
    if ( filePath.isEmpty( ) )
    {
      return nullptr;
    }

    QFileInfo fileInfo( filePath );

    //Format basename to 10 chars without spaces
    std::string basename = fileInfo.baseName( ).toStdString( );
    basename.erase( std::remove_if( basename.begin( ), basename.end( ),
      isspace ), basename.end( ) );
    if (basename.length() > MAX_DATASET_NAME_LENGTH )
    {
      basename.resize ( MAX_DATASET_NAME_LENGTH );
    }

    std::string name;
    std::string path = filePath.toStdString( );
    std::string tempName = basename;
    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      QString dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name: " + filePath, QString::fromStdString( tempName ),
        regularExpression, &validName);
      if ( !validName )
      {
        return nullptr;
      }

      name = dataSetName.toStdString( );

      //Check if name doesn't exist
      notUsedName = true;
      for( int i = 0; i < count( ); ++i )
      {
        DataSetWidget* widget = static_cast< DataSetWidget* >(
          itemWidget( item( i ) ) );
        if ( widget->getName( ) == name )
        {
          QMessageBox::warning( this, APPLICATION_NAME,
            tr("Invalid name. This name already exists.") );
          notUsedName = false;
        }
      }

    } while( !notUsedName );

    //Add to dataset
    DataSetWidget* dsw = new DataSetWidget( name, path );
    dsw->setListWidgetItem( new QListWidgetItem( this ) );

    QListWidgetItem* listWidgetItem = dsw->getListWidgetItem( );
    addItem( listWidgetItem );
    listWidgetItem->setSizeHint( dsw->sizeHint ( ) );
    setItemWidget( listWidgetItem, dsw );

    return dsw;
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

      DataSetPtr dataSet( new DataSet( dsw->getPath( ), dsw->getChecked( ),
        dsw->getHeaders( ) ) );
      dataSets[ dsw->getName() ] = dataSet;

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
    for( const QUrl& url : event->mimeData()->urls( ) )
    {
      QString filePath = url.toLocalFile();

      std::string extension = sp1common::Strings::lower(
        QFileInfo(filePath).completeSuffix( ).toStdString( ) );

      if ( extension == "csv" )
      {
        sp1common::Debug::consoleMessage( "Dropped file: "
          + filePath.toStdString( ) );
        emit addDataSetEvent( filePath.toStdString( ) );
      }
      else
      {
        sp1common::Debug::consoleMessage( "Ignoring invalid file: "
          + filePath.toStdString( ) );
      }
    }
  }
}
