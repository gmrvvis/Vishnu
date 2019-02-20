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

#include "UserDataSetListWidget.h"

#include <algorithm>

#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsBlurEffect>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{
  UserDataSetListWidget::UserDataSetListWidget( QWidget* /*parent*/ )
    : _checkingProperty( false )
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
  }

  UserDataSetWidgetPtr UserDataSetListWidget::addUserDataSet(
    const std::string& name, const std::string& path,
    const std::string& csvFilename, const std::string& jsonFilename,
    const std::string& xmlFilename, const bool& selected )
  {
    //UserDataSetWidgets dataSetWidgets;
/*
    //Format basename to 10 chars without spaces
    std::string name;
    std::string tempName = std::string( );
    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      std::string dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name: " + filePath, std::string::fromStdString( tempName ),
        regularExpression, &validName);
      if ( validName )
      {
        name = dataSetName.toStdString( );
        //Check if name doesn't exist
        notUsedName = true;
        for( int j = 0; j < count( ); ++j )
        {
          UserDataSetWidget* widget = static_cast< UserDataSetWidget* >(
            itemWidget( item( j ) ) );
          if ( widget->getName( ) == name )
          {
            QMessageBox::warning( this, APPLICATION_NAME,
              tr("Invalid name. This name already exists.") );
            notUsedName = false;
            break;
          }
        }
      }
    } while( !validName || !notUsedName );
*/
    //Add to dataset
    UserDataSetWidgetPtr dataSetWidget( new UserDataSetWidget( name, path,
      csvFilename, jsonFilename, xmlFilename, selected ) );
    dataSetWidget->setListWidgetItem( new QListWidgetItem( this ) );

    QListWidgetItem* listWidgetItem = dataSetWidget->getListWidgetItem( );
    addItem( listWidgetItem );
    listWidgetItem->setSizeHint( dataSetWidget->sizeHint ( ) );
    setItemWidget( listWidgetItem, dataSetWidget );

    return dataSetWidget;
  }

  void UserDataSetListWidget::removeCurrentDataSet( )
  {
    takeItem( row( currentItem( ) ) );
  }

  void UserDataSetListWidget::selectCurrentDataSet( void )
  {
    UserDataSetWidgetPtr currentDataSet =
      static_cast< UserDataSetWidgetPtr >( itemWidget( currentItem( ) ) );

    if ( !_checkingProperty )
    {
      _checkingProperty = true;
      for( int row = 0; row < count( ); ++row )
      {
        UserDataSetWidget* dataSet = static_cast< UserDataSetWidget* >(
          itemWidget( item( row ) ) );
        if ( currentDataSet != dataSet )
        {
          dataSet->setSelected( false );
        }
      }
      currentDataSet->setSelected( true );
      _checkingProperty = false;
    }
  }

  std::string UserDataSetListWidget::getCurrentDataSetName( )
  {
    UserDataSetWidgetPtr userDataSet =
      static_cast< UserDataSetWidgetPtr >( itemWidget( currentItem( ) ) );
    return userDataSet->getName( );
  }

  UserDataSetPtr UserDataSetListWidget::getCurrentDataSet( )
  {
    UserDataSetWidgetPtr dsw = static_cast< UserDataSetWidgetPtr >(
      itemWidget( currentItem( ) ) );
    UserDataSetPtr userDataSet( new UserDataSet( dsw->getName( ), dsw->getPath( ),
      dsw->getCsvFilename( ), dsw->getJsonFilename( ), dsw->getXmlFilename( ),
      dsw->getSelected( ) ) );
    return userDataSet;
  }

  UserDataSetMap UserDataSetListWidget::getDataSets( )
  {
    UserDataSetMap dataSets;

    for( int row = 0; row < count( ); ++row )
    {
      UserDataSetWidgetPtr dsw = static_cast< UserDataSetWidgetPtr >(
        itemWidget( item( row ) ) );

      UserDataSetPtr dataSet( new UserDataSet( dsw->getName( ), dsw->getPath( ),
        dsw->getCsvFilename( ), dsw->getJsonFilename( ), dsw->getXmlFilename( ),
        dsw->getSelected( ) ) );
      dataSets[ dsw->getName( ) ] = dataSet;

    }

    return dataSets;
  }

  void UserDataSetListWidget::clearDataSets( void )
  {
    clear( );
  }

  void UserDataSetListWidget::dragMoveEvent( QDragMoveEvent* event )
  {
    event->acceptProposedAction();
  }

  void UserDataSetListWidget::dragLeaveEvent( QDragLeaveEvent* event )
  {
    event->accept();
  }

  void UserDataSetListWidget::setBlurred( const bool& state )
  {

    for ( int row = 0; row < count( ); ++row )
    {
      if ( state )
      {
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect( this );
        itemWidget( item( row ) )->setGraphicsEffect( blur );
      }
      else
      {
        itemWidget( item( row ) )->setGraphicsEffect( 0 );
      }
    }
  }

}
