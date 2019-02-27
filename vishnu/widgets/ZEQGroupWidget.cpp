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

#include "ZEQGroupWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QFocusEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QColor>

#include <sstream>
#include <iostream>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{

  ZEQGroupWidget::ZEQGroupWidget( const std::string& key, const QString& name,
    const QString& owner, const std::vector< std::string >& ids,
    const QColor& color, QWidget* /*parent*/ )
     : _key( key )
     , _ids( ids )
  {
    QHBoxLayout *hLayout = new QHBoxLayout( this );

    //DataSet image
    QVBoxLayout *vLayout1 = new QVBoxLayout( );
    setColor( color );
    vLayout1->addWidget( _color, 0, 0 );
    hLayout->addLayout( vLayout1, 0 );
    hLayout->addSpacing( 30 );

    setName( name );
    setOwner( owner );

    hLayout->addWidget( _name, 1 );
    hLayout->addWidget( _owner, 2 );
    hLayout->addSpacing( 30 );

    //DataSet remove image
    QLabel* removeGroupImage = new QLabel( );
    removeGroupImage->setStyleSheet("width: 32px; height: 32px;" );
    QPixmap removeGroupPixmap( ":/icons/close.png" );
    removeGroupImage->setPixmap( removeGroupPixmap );
    _remove = new QPushButton( );
    _remove->setIcon( QIcon( ":/icons/close.png" ) );
    _remove->setStyleSheet( "QWidget:hover{background-color:#AAAAFF;}" );
    QObject::connect( _remove, SIGNAL( clicked( ) ), this,
      SLOT( clickRemove( ) ) );

    QVBoxLayout *vLayout4 = new QVBoxLayout( );
    vLayout4->addWidget( _remove, 0, 0);
    hLayout->addLayout( vLayout4, 0 );

    setLayout( hLayout );

    show( );
  }

  std::string ZEQGroupWidget::getKey( void ) const
  {
    return _key;
  }

  QString ZEQGroupWidget::getName( ) const
  {
    return _name->text( );
  }

  void ZEQGroupWidget::setName( const QString& name )
  {
    if ( _name == nullptr)
    {
      _name = new QLabel( );
    }
    _name->setText( name );
    _name->setStyleSheet( "font-weight: bold; font-size: 12px" );
  }

  QString ZEQGroupWidget::getOwner( ) const
  {
    return _owner->text( );
  }

  void ZEQGroupWidget::setOwner( const QString& owner )
  {
    if ( _owner == nullptr)
    {
      _owner = new QLabel( );
    }
    _owner->setText( owner );
    _owner->setStyleSheet( "font-weight: bold; font-size: 12px" );
  }

  QColor ZEQGroupWidget::getColor( void ) const
  {
    return _color->pixmap( )->toImage( ).pixelColor( 0, 0 );
  }

  void ZEQGroupWidget::setColor( const QColor& color )
  {
    if ( _color == nullptr)
    {
      _color = new QLabel( );
    }

    _color->setStyleSheet( "width: 32px; height: 32px;" );
    QPixmap pixmap( ":/icons/blank.png" );
    pixmap.fill( color );
    _color->setPixmap( pixmap );
  }

  std::vector< std::string > ZEQGroupWidget::getIds( void ) const
  {
    return _ids;
  }

  void ZEQGroupWidget::setIds( const std::vector< std::string >& ids )
  {
    _ids = ids;
  }

  void ZEQGroupWidget::clickRemove( void )
  {
    _listWidgetItem->listWidget()->setCurrentItem( _listWidgetItem );
    emit signalRemoveSelectedGroup( );
  }

  QListWidgetItem* ZEQGroupWidget::getListWidgetItem() const
  {
    return _listWidgetItem;
  }

  void ZEQGroupWidget::setListWidgetItem( QListWidgetItem* listWidgetItem )
  {
    _listWidgetItem = listWidgetItem;
  }

}
