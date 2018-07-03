#include "PropertiesWidget.h"

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

  PropertiesWidget::PropertiesWidget( const std::string& name, const bool& use,
        const bool& primaryKey, const sp1common::DataType& dataType,
        const AxisType& axisType, QWidget* /*parent*/ )
  {    

    fillDataTypes( );
    fillAxisTypes( );
    setName( name );
    setUse( use );
    setPrimaryKey( primaryKey );
    setDataType( dataType );
    setAxisType( axisType );

    QObject::connect( _use, SIGNAL( toggled( bool ) ), this,
      SLOT( useChanged( bool ) ) );

    QObject::connect( _primaryKey, SIGNAL( toggled( bool ) ), this,
      SLOT( primaryKeyChanged( bool ) ) );

    QObject::connect( _axisType, SIGNAL( currentTextChanged( QString ) ), this,
      SLOT( axisTypeChanged( QString ) ) );

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

  void PropertiesWidget::fillDataTypes( void )
  {
    if ( _dataType == nullptr)
    {
      _dataType = new QComboBox( );
    }
    for (const auto& dt : sp1common::dataTypesToVector( ) )
    {
      _dataType->addItem( QString::fromStdString( dt ) );
    }
  }

  sp1common::DataType PropertiesWidget::getDataType( void ) const
  {
    return sp1common::toDataType( _dataType->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setDataType( const sp1common::DataType& dataType )
  {
    int index = _dataType->findData( QString::fromStdString(
      sp1common::toString( dataType ) ) );
    if ( index != -1 )
    {
      _dataType->setCurrentIndex( index );
    }
  }

  void PropertiesWidget::fillAxisTypes( void )
  {
    if ( _axisType == nullptr )
    {
      _axisType = new QComboBox( );
    }
    for ( const auto& at : axisTypesToVector( ) )
    {
      _axisType->addItem( QString::fromStdString( at ) );
    }
  }

  AxisType PropertiesWidget::getAxisType( void ) const
  {
    return toAxisType( _axisType->currentText( ).toStdString( ) );
  }

  void PropertiesWidget::setAxisType( const AxisType& axisType )
  {
    int index = _axisType->findData( QString::fromStdString(
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
        return _dataType;
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

  void PropertiesWidget::axisTypeChanged( QString text )
  {
    //TODO: switch text, loop over all properties changing comboboxes
    AxisType axisType = toAxisType( text.toStdString( ) );
    switch ( axisType )
    {
      case AxisType::None:
        //Check removed option, and re-enable on other properties
        //If X, Y or Z removed, enable removed option on all properties and 
        //check if 3 are removed to enable XYZ
        //If XYZ removed, enable X, Y, Z and XYZ on all properties
        break;
      case AxisType::X:
        //Loop over properties removing X and XYZ options
        break;
      case AxisType::Y:
        //Loop over properties removing Y and XYZ options
        break;
      case AxisType::Z:
        //Loop over properties removing Z and XYZ options
        break;
      case AxisType::XYZ:
        //Loop over properties removing X, Y, Z and XYZ options
        break;
    }
  }

}
