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

#include "ZEQGroupListWidget.h"

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

#include <QGraphicsBlurEffect>

namespace vishnu
{
  ZEQGroupListWidget::ZEQGroupListWidget( QWidget* /*parent*/ )
  {    
    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::DragDropMode::InternalMove );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );

    setMouseTracking( true );
    setStyleSheet(
      "QListWidget::item:selected{border: 1px solid #6a6a6a;background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 0.5 #FFFFFF, stop: 1 #AAAAFF );}"
      "QListWidget::item:!selected{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #CCCCFF, stop: 0.5 #FFFFFF, stop: 1.0 #CCCCFF );}"
      "QListWidget::item:hover{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #BBBBFF, stop: 0.5 #FFFFFF, stop: 1 #BBBBFF );}"
    );

    //QStyle* style = new QStyle( );
    //setStyle()

    //item()
    //ite :hover { background: transparent; }

    //setAlternatingRowColors( true );
    //setStyleSheet("alternate-background-color: white;background-color: red;");

    //setDropIndicatorShown( true );
    //setMovement( QListView::Movement::Snap );
    //setUniformItemSizes( true);
  }

  ZEQGroupWidgetPtr ZEQGroupListWidget::syncGroup( const std::string& key,
    const QString& name, const QString& owner,
    const std::vector< std::string >& ids, const QColor& color )
  {
    ZEQGroupWidgetPtr zeqGroup = findGroup( key );

    if ( zeqGroup )
    {
      //Group exists -> Update group
      zeqGroup->setName( name );
      zeqGroup->setOwner( owner );
      zeqGroup->setIds( ids );
      zeqGroup->setColor( color );
      vishnucommon::Debug::consoleMessage("Group: '" + key +
        "' updated successfully.");
    }
    else
    {
      //Create new group
      zeqGroup = new ZEQGroupWidget( key, name, owner, ids, color );

      QListWidgetItem* listWidgetItem = new QListWidgetItem( this );
      zeqGroup->setListWidgetItem( listWidgetItem );
      addItem( listWidgetItem );
      listWidgetItem->setSizeHint( zeqGroup->sizeHint ( ) );
      setItemWidget( listWidgetItem, zeqGroup );

      vishnucommon::Debug::consoleMessage("Group: '" + key +
        "' created successfully.");
    }

    return zeqGroup;
  }

  void ZEQGroupListWidget::setBlurred( const bool& state )
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

  ZEQGroupWidgetPtr ZEQGroupListWidget::findGroup( const std::string& key )
  {
    for( int row = 0; row < count( ); ++row )
    {
      ZEQGroupWidgetPtr zeqGroup = static_cast< ZEQGroupWidgetPtr >( itemWidget(
        item( row ) ) );

      if ( zeqGroup->getKey( ) == key )
      {
        return zeqGroup;
      }
    }
    return nullptr;
  }

  std::string ZEQGroupListWidget::getKey( QListWidgetItem* listWidgetItem )
  {
    ZEQGroupWidgetPtr zeqGroup = static_cast< ZEQGroupWidgetPtr >( itemWidget(
      listWidgetItem ) );
    return zeqGroup->getKey( );
  }

  void ZEQGroupListWidget::changeGroupName( const std::string& key,
    const QString& name )
  {
    ZEQGroupWidget* zeqGroup = findGroup( key );
    if ( zeqGroup )
    {
      std::string oldName = zeqGroup->getName( ).toStdString( );
      zeqGroup->setName( name );

      vishnucommon::Debug::consoleMessage( "Group: '" + key
        + "' - Group name changed sucessfully ('" + oldName + "' -> '"
        + name.toStdString( ) + "')." );
    }
  }

  void ZEQGroupListWidget::changeGroupColor( const std::string& key,
    const QColor& color )
  {
    ZEQGroupWidgetPtr zeqGroup = findGroup( key );
    if ( zeqGroup )
    {
      //int r,g,b;
      //color.getRgb( &r, &g, &b );
      QColor oldColor = zeqGroup->getColor( );
      zeqGroup->setColor( color );

      vishnucommon::Debug::consoleMessage( "Group: '" + key
        + "' - Group color changed sucessfully ('"
        + oldColor.name( ).toStdString( ) + "' -> '"
        + color.name( ).toStdString( ) + "')." );
    }
  }

  void ZEQGroupListWidget::removeCurrentGroup( )
  {    
    QListWidgetItem* listWidgetItem = currentItem( );
    ZEQGroupWidgetPtr zeqGroup = static_cast< ZEQGroupWidgetPtr >( itemWidget(
      listWidgetItem ) );
    std::string key = zeqGroup->getKey( );

    takeItem( row( listWidgetItem ) );

    vishnucommon::Debug::consoleMessage("Group: '" + key
      + "' removed successfully.");
  }

  void ZEQGroupListWidget::removeGroup( const std::string& key )
  {
    for( int row = 0; row < count( ); ++row )
    {
      ZEQGroupWidgetPtr zeqGroup = static_cast< ZEQGroupWidgetPtr >( itemWidget(
        item( row ) ) );

      if ( zeqGroup->getKey( ) == key )
      {
        takeItem( row );

        vishnucommon::Debug::consoleMessage("Group: '" + key
          + "' removed successfully.");
      }
    }
  }

}
