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

#include "PropertiesWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QFocusEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QColor>

#include <iostream>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{

  PropertiesWidget::PropertiesWidget( const std::string& name, const bool& use,
        const bool& primaryKey, const vishnucommon::DataCategory& dataCategory,
        const vishnucommon::AxisType& axisType, QWidget* /*parent*/ )
  {    

    fillDataCategories( );
    fillAxisTypes( );
    setName( name );
    setUse( use );
    setPrimaryKey( primaryKey );
    setDataCategory( dataCategory );
    setAxisType( axisType );

    _use->setStyleSheet(
      //"QCheckBox { background-color: #AAAAAA; }"
      "QCheckBox::indicator { width: 32px; height: 32px; subcontrol-position: center center;}"
      "QCheckBox::indicator:checked { background-image:url(:/icons/checked.png); }"
      "QCheckBox::indicator:unchecked { background-image:url(:/icons/unchecked.png); }"
    );

    _primaryKey->setStyleSheet(
      //"QCheckBox { background-color: #AAAAAA; }"
      "QCheckBox::indicator { width: 32px; height: 32px; subcontrol-position: center center;}"
      "QCheckBox::indicator:checked { background-image:url(:/icons/checked.png); }"
      "QCheckBox::indicator:unchecked { background-image:url(:/icons/unchecked.png); }"
    );

    QObject::connect( _use, SIGNAL( toggled( bool ) ), this,
      SLOT( useChanged( bool ) ) );

    QObject::connect( _primaryKey, SIGNAL( toggled( bool ) ), this,
      SLOT( primaryKeyChanged( bool ) ) ); 
  }

  std::string PropertiesWidget::getName( void ) const
  {
    return _name->text( ).toStdString( );
  }

  void PropertiesWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new QLabel( );
    }
    _name->setText( QString::fromStdString( name ) );
  }

  bool PropertiesWidget::getUse( void ) const
  {
    return _use->isChecked( );
  }

  void PropertiesWidget::setUse( const bool& use )
  {
    if ( _use == nullptr)
    {
      _use = new QCheckBox( );
    }
    _use->setChecked( use );
  }

  bool PropertiesWidget::getPrimaryKey( void ) const
  {
    return _primaryKey->isChecked( );
  }

  void PropertiesWidget::setPrimaryKey( const bool& primaryKey )
  {
    if ( _primaryKey == nullptr)
    {
      _primaryKey = new QCheckBox( );
    }
    _primaryKey->setChecked( primaryKey );
  }

  void PropertiesWidget::fillDataCategories( void )
  {
    if ( _dataCategory == nullptr)
    {
      _dataCategory = new QComboBox( );
    }
    for (const auto& dt : vishnucommon::dataCategoriesToVector( ) )
    {
      _dataCategory->addItem( QString::fromStdString( dt ) );
    }
  }

  vishnucommon::DataCategory PropertiesWidget::getDataCategory( void ) const
  {
    return vishnucommon::toDataCategory(
      _dataCategory->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setDataCategory(
    const vishnucommon::DataCategory& dataCategory )
  {
    int index = _dataCategory->findText( QString::fromStdString(
      vishnucommon::toString( dataCategory ) ) );

    if ( index != -1 )
    {
      _dataCategory->setCurrentIndex( index );
    }
  }

  void PropertiesWidget::fillAxisTypes( void )
  {
    if ( _axisType == nullptr )
    {
      _axisType = new QComboBox( );
    }
    for ( const auto& at : vishnucommon::axisTypesToVector( ) )
    {
      _axisType->addItem( QString::fromStdString( at ) );
    }
  }

  vishnucommon::AxisType PropertiesWidget::getAxisType( void ) const
  {
    return vishnucommon::toAxisType( _axisType->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setAxisType( const vishnucommon::AxisType& axisType )
  {
    int index = _axisType->findText( QString::fromStdString(
      toString( axisType ) ) );

    if ( index != -1 )
    {
      _axisType->setCurrentIndex( index );
    }
  }

  QWidget* PropertiesWidget::getWidget( const int& index )
  {
    switch( index )
    {
      case 0:
        return _name;
      case 1:
        return _use;
      case 2:
        return _primaryKey;
      case 3:
        return _dataCategory;
      case 4:
        return _axisType;
      default:
        vishnucommon::Error::throwError(vishnucommon::Error::ErrorType::Error,
          "Invalid property index", true );
        return nullptr;
    }
  }

  void PropertiesWidget::useChanged( bool state )
  {
    if ( !state )
    {
      if ( _primaryKey->isChecked( ) )
      {
        _primaryKey->setChecked( state );
      }
    }
  }

  void PropertiesWidget::primaryKeyChanged( bool state )
  {
    if ( state )
    {
      if ( !_use->isChecked( ) )
      {
        _use->setChecked( state );
      }
    }
  }

}
