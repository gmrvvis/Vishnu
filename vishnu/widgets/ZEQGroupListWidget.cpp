/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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

namespace vishnu
{
  ZEQGroupListWidget::ZEQGroupListWidget( QWidget* /*parent*/ )
  {    
    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::DragDrop );
    setDefaultDropAction( Qt::MoveAction );
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
      sp1common::Debug::consoleMessage("Group: '" + key +
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

      sp1common::Debug::consoleMessage("Group: '" + key +
        "' created successfully.");
    }

    return zeqGroup;
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

      sp1common::Debug::consoleMessage( "Group: '" + key
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

      sp1common::Debug::consoleMessage( "Group: '" + key
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

    sp1common::Debug::consoleMessage("Group: '" + key
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

        sp1common::Debug::consoleMessage("Group: '" + key
          + "' removed successfully.");
      }
    }
  }

}
