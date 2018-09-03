/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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
        const bool& primaryKey, const sp1common::DataCategory& dataCategory,
        const sp1common::AxisType& axisType, QWidget* /*parent*/ )
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
    for (const auto& dt : sp1common::dataCategoriesToVector( ) )
    {
      _dataCategory->addItem( QString::fromStdString( dt ) );
    }
  }

  sp1common::DataCategory PropertiesWidget::getDataCategory( void ) const
  {
    return sp1common::toDataCategory(
      _dataCategory->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setDataCategory(
    const sp1common::DataCategory& dataCategory )
  {
    int index = _dataCategory->findText( QString::fromStdString(
      sp1common::toString( dataCategory ) ) );

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
    for ( const auto& at : sp1common::axisTypesToVector( ) )
    {
      _axisType->addItem( QString::fromStdString( at ) );
    }
  }

  sp1common::AxisType PropertiesWidget::getAxisType( void ) const
  {
    return sp1common::toAxisType( _axisType->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setAxisType( const sp1common::AxisType& axisType )
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
        sp1common::Error::throwError(sp1common::Error::ErrorType::Error,
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
