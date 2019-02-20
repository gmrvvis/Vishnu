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

#include "PropertiesTableWidget.h"

#include <algorithm>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QHeaderView>

namespace vishnu
{
  PropertiesTableWidget::PropertiesTableWidget( QWidget* /*parent*/ )
    : _checkingProperty( false )
    , _sourceIndex( -1 )
  {
    setColumnCount( 5 );
    setColumnWidth( 0, 300 );
    setColumnWidth( 1, 100 );
    setColumnWidth( 2, 100 );
    setColumnWidth( 3, 140 );
    setColumnWidth( 4, 50 );

    horizontalHeader()->setStretchLastSection( true );
    QStringList headers;
    headers << "Property name" << "Use" << "Primary Key" << "Data category"
      << "Axis type";
    setHorizontalHeaderLabels( headers );
    setSortingEnabled( true );
    verticalHeader( )->setVisible( false );

    setDragEnabled( true );
    setSelectionMode( QAbstractItemView::SelectionMode::SingleSelection );
    setDragDropMode( QAbstractItemView::DragDropMode::InternalMove );
    setSelectionBehavior( QAbstractItemView::SelectionBehavior::SelectRows );
    setDropIndicatorShown( true );

  }

  void PropertiesTableWidget::checkPrimaryKeys(
    const std::vector< std::string >& commonProperties )
  {
    for( int row = 0; row < rowCount( ); ++row )
    {
      //Columns 0, 2 -> name, pk
      QLabel* label = static_cast< QLabel* >( cellWidget( row, 0 ) );
      QCheckBox* primaryKey = static_cast< QCheckBox* >( cellWidget( row, 2 ) );

      if ( std::find(commonProperties.begin(), commonProperties.end(),
        label->text( ).toStdString( ) ) != commonProperties.end())
      {
        cellWidget( row, 2 )->setEnabled(true);
      }
      else
      {
        primaryKey->setChecked( false );
        cellWidget( row, 2 )->setDisabled(true);
      }
    }
  }

  void PropertiesTableWidget::addProperties(
    const vishnucommon::Properties& properties,
    const vishnucommon::PropertyGroupsPtr& propertyGroups )
  {
    for ( const auto& property : properties )
    {
      std::string propertyName = property->getName( );
      std::vector< std::string > tableProperties;
      for (int i = 0; i < rowCount( ); ++i )
      {
        tableProperties.emplace_back( static_cast< QLabel* >( cellWidget( i, 0 )
          )->text( ).toStdString( ) );
      }

      if ( std::find( tableProperties.begin( ), tableProperties.end( ),
        propertyName ) == tableProperties.end( ) )
      {
        std::vector< std::string > usedPrimaryKeys =
          propertyGroups->getUsedPrimaryKeys( );
        std::vector< std::string > usedNonPrimaryKeys =
          propertyGroups->getUsedNonPrimaryKeys( );
        std::vector< std::string > axes = propertyGroups->getAxes( );
        bool isPrimaryKey = false;
        bool inUse = false;
        vishnucommon::AxisType axis = vishnucommon::AxisType::None;
        if ( vishnucommon::Vectors::find( usedPrimaryKeys, propertyName ) != -1 )
        {
          isPrimaryKey = true;
          inUse = true;
        }
        else if ( vishnucommon::Vectors::find( usedNonPrimaryKeys, propertyName )
          != -1 )
        {
          inUse = true;
        }
        int axisPos = vishnucommon::Vectors::find( axes, propertyName );
        if ( axisPos != -1 )
        {
          if ( axes.size( ) > 1 )
          {
            switch( axisPos )
            {
              case 0:
                axis = vishnucommon::AxisType::X;
                break;
              case 1:
                axis = vishnucommon::AxisType::Y;
                break;
              case 2:
                axis = vishnucommon::AxisType::Z;
                break;
              default:
                vishnucommon::Error::throwError(
                  vishnucommon::Error::ErrorType::Error, "Invalid Axis", true );
                break;
            }
          }
          else
          {
            axis = vishnucommon::AxisType::XYZ;
          }
        }

        PropertiesWidget* propertiesWidget = new PropertiesWidget( propertyName,
          inUse, isPrimaryKey, property->getDataCategory( ), axis );

        int row = rowCount( );
        int columns = columnCount( );
        insertRow( row );
        for( int column = 0; column < columns; ++column )
        {
          setItem( row, column, new QTableWidgetItem( ) );
          setCellWidget( row, column, propertiesWidget->getWidget( column ) );
        }

        //DataCategory
        QObject::connect( propertiesWidget->getWidget( 3 ),
          SIGNAL( currentIndexChanged( QString ) ), this,
          SLOT( dataCategoryChanged( QString ) ) );

        //Axis Type
        QObject::connect( propertiesWidget->getWidget( 4 ),
          SIGNAL( currentIndexChanged( QString ) ), this,
          SLOT( axisTypeChanged( QString ) ) );
      }
    }
  }

  void PropertiesTableWidget::refillAxisTypeComboBox( QComboBox* combo,
    const vishnucommon::AxisType& axisType,
    const std::vector< vishnucommon::AxisType >& toExclude )
  {
    combo->clear( );
    for ( const auto& at : axisTypesToVector( toExclude ) )
    {
      combo->addItem( QString::fromStdString( at ) );
    }
    combo->setCurrentText( QString::fromStdString( toString( axisType ) ) );
  }

  void PropertiesTableWidget::changeToNoneOrXOrYOrZ(
    const std::vector< vishnucommon::AxisType >& selectedAxis )
  {
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      vishnucommon::AxisType axisType = vishnucommon::toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< vishnucommon::AxisType > toExclude = selectedAxis;
      vishnucommon::Vectors::remove( toExclude, axisType );

      if ( ( ( vishnucommon::Vectors::find( toExclude, vishnucommon::AxisType::X ) != -1 )
        || ( vishnucommon::Vectors::find( toExclude, vishnucommon::AxisType::Y ) != -1 )
        || ( vishnucommon::Vectors::find( toExclude, vishnucommon::AxisType::Z ) != -1 ) )
        && ( vishnucommon::Vectors::find( toExclude, vishnucommon::AxisType::XYZ ) == -1 ) )
      {
        toExclude.push_back( vishnucommon::AxisType::XYZ );
      }

      refillAxisTypeComboBox( axisCB, axisType, toExclude );
    }
  }

  void PropertiesTableWidget::changeToXYZ(
    const std::vector< vishnucommon::AxisType >& selectedAxis )
  {
    //Loop over properties removing X, Y, Z and XYZ options
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      vishnucommon::AxisType axisType = vishnucommon::toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< vishnucommon::AxisType > toExclude = selectedAxis;
      vishnucommon::Vectors::remove( toExclude, axisType );

      if ( vishnucommon::Vectors::find( toExclude, vishnucommon::AxisType::XYZ )
        != -1 )
      {
        toExclude.push_back( vishnucommon::AxisType::X );
        toExclude.push_back( vishnucommon::AxisType::Y );
        toExclude.push_back( vishnucommon::AxisType::Z );
      }

      refillAxisTypeComboBox( axisCB, axisType, toExclude );
    }
  }

  void PropertiesTableWidget::axisTypeChanged( QString text )
  {
    if ( !_checkingProperty  )
    {
      _checkingProperty = true;

      /* First:
       Loop over properties in order to get selected items, must be one of:
      - All "blank" (ignored)
      - X and/or Y and/or Z (non repeated)
      - XYZ (non repeated)
       */
      std::vector< vishnucommon::AxisType > selectedAxis;
      for (int i = 0; i < rowCount( ); ++i )
      {
        QComboBox* cb = static_cast< QComboBox* >( cellWidget( i, 4 ) );
        vishnucommon::AxisType at = vishnucommon::toAxisType(
          cb->currentText( ).toStdString( ) );
        if ( at != vishnucommon::AxisType::None )
        {
          selectedAxis.push_back( at );
        }
      }

      /* Second:
       Get sender combobox and loop over all combos excluding some results
      */
      vishnucommon::AxisType senderAxisType =
        vishnucommon::toAxisType( text.toStdString( ) );
      switch ( senderAxisType )
      {
        case vishnucommon::AxisType::None:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case vishnucommon::AxisType::X:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case vishnucommon::AxisType::Y:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case vishnucommon::AxisType::Z:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case vishnucommon::AxisType::XYZ:
          changeToXYZ( selectedAxis );
          break;
      }
      _checkingProperty = false;
    }
  }

  void PropertiesTableWidget::dataCategoryChanged( QString text )
  {
    if ( vishnucommon::toDataCategory( text.toStdString( ) )
      == vishnucommon::DataCategory::Geometric )
    {
        QComboBox* cbSender = static_cast< QComboBox* >( sender( ) );
        if ( !_checkingProperty  )
        {
          _checkingProperty = true;

          for (int i = 0; i < rowCount( ); ++i )
          {
            QComboBox* cb = static_cast< QComboBox* >( cellWidget( i, 3 ) );
            if ( cb != cbSender )
            {
              if ( vishnucommon::toDataCategory( cb->currentText( ).toStdString( ) )
                == vishnucommon::DataCategory::Geometric )
              {
                cb->setCurrentText( QString::fromStdString( toString(
                  vishnucommon::DataCategory::Undefined ) ) );
              }
            }
          }

          _checkingProperty = false;
        }
    }
  }

  void PropertiesTableWidget::removeProperties(
    const std::vector< std::string >& properties )
  {
    for ( const auto& property : properties )
    {
      for( int row = 0; row < rowCount( ); ++row )
      {
        //Column 0 -> name
        QLabel* label = static_cast< QLabel* >( cellWidget( row, 0 ) );

        if ( label->text( ) == QString::fromStdString( property ) )
        {
          removeRow( row );
        }
      }
    }
  }

  vishnucommon::DataSetsPtr PropertiesTableWidget::getDataSets( void )
  {
    vishnucommon::DataSetsPtr dataSets( new vishnucommon::DataSets( ) );
    vishnucommon::DataSetPtr dataSet( new vishnucommon::DataSet( ) );
    vishnucommon::PropertyGroupsPtr propertyGroups(
      new vishnucommon::PropertyGroups( ) );
    std::string xAxis;
    std::string yAxis;
    std::string zAxis;
    std::string xyzAxis;
    for( int row = 0; row < rowCount( ); ++row )
    {
      std::string name = static_cast< QLabel* >( cellWidget( row, 0 )
        )->text( ).toStdString( );
      bool use = static_cast< QCheckBox* >( cellWidget( row, 1 )
        )->isChecked( );
      bool primaryKey = static_cast< QCheckBox* >( cellWidget( row, 2 )
        )->isChecked( );
      QComboBox* dataTypeComboBox = static_cast< QComboBox* >( cellWidget(
        row, 3 ) );
      vishnucommon::DataCategory dataCategory = vishnucommon::toDataCategory(
        dataTypeComboBox->currentText( ).toStdString( ) );
      QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
        row, 4 ) );
      vishnucommon::AxisType axisType = vishnucommon::toAxisType(
        axisTypeComboBox->currentText( ).toStdString( ) );

      if ( use )
      {
        if ( primaryKey )
        {
          propertyGroups->addUsedPrimaryKey( name );
        }
        else
        {
          propertyGroups->addUsedNonPrimaryKey( name );
        }
        dataSet->addProperty( vishnucommon::PropertyPtr(
          new vishnucommon::Property( name, dataCategory,
          vishnucommon::DataStructureType::None ) ) );
      }

      switch( axisType )
      {
        case vishnucommon::AxisType::X:
          xAxis = name;
          break;
        case vishnucommon::AxisType::Y:
          yAxis = name;
          break;
        case vishnucommon::AxisType::Z:
          zAxis = name;
          break;
        case vishnucommon::AxisType::XYZ:
          xyzAxis = name;
          break;
        default:
          break;
      }
    }

    if ( !xyzAxis.empty( ) )
    {
      propertyGroups->setAxes( xyzAxis );
    }
    else
    {
      propertyGroups->setAxes( xAxis, yAxis, zAxis );
    }

    dataSets->setDataSets( { dataSet } );
    dataSets->setPropertyGroups( propertyGroups );

    return dataSets;
  }

  void PropertiesTableWidget::dragEnterEvent( QDragEnterEvent* event )
  {
    int index = currentIndex( ).row( );
    if ( index >= 0 )
    {
      _sourceIndex = index;
      event->acceptProposedAction( );
    }
  }

  void PropertiesTableWidget::dragMoveEvent( QDragMoveEvent* event )
  {
    event->acceptProposedAction( );
  }

  void PropertiesTableWidget::dragLeaveEvent( QDragLeaveEvent* event )
  {
    event->accept( );
  }

  void PropertiesTableWidget::dropEvent( QDropEvent* event )
  {
    if ( _sourceIndex != -1 )
    {
      int targetIndex = indexAt( event->pos( ) ).row( );

      PropertiesWidget* sourceRow = getRow( _sourceIndex );
      PropertiesWidget* targetRow = getRow( targetIndex );

      for( int column = 0; column < columnCount( ); ++column )
      {
        setCellWidget( _sourceIndex, column, targetRow->getWidget( column ) );
        setCellWidget( targetIndex, column, sourceRow->getWidget( column ) );
      }

      _sourceIndex = -1;

      //Set moved row as selected row
      setCurrentCell( targetIndex, 0 );

      event->acceptProposedAction( );
    }

  }

  PropertiesWidget* PropertiesTableWidget::getRow( const int& index )
  {
    if ( ( index >= rowCount( ) ) || ( index < 0 ) )
    {
      vishnucommon::Error::throwError( vishnucommon::Error::ErrorType::Error,
        "Invalid row", true );
    }

    std::string name = static_cast< QLabel* >( cellWidget( index, 0 )
      )->text( ).toStdString( );
    bool use = static_cast< QCheckBox* >( cellWidget( index, 1 )
      )->isChecked( );
    bool primaryKey = static_cast< QCheckBox* >( cellWidget( index, 2 )
      )->isChecked( );
    QComboBox* dataCategoryComboBox = static_cast< QComboBox* >( cellWidget(
      index, 3 ) );
    vishnucommon::DataCategory dataCategory = vishnucommon::toDataCategory(
      dataCategoryComboBox->currentText( ).toStdString( ) );
    QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
      index, 4 ) );
    vishnucommon::AxisType axisType = vishnucommon::toAxisType(
      axisTypeComboBox->currentText( ).toStdString( ) );

    PropertiesWidget* propertiesWidget = new PropertiesWidget( name, use,
      primaryKey, dataCategory, axisType );

    return propertiesWidget;
  }

}
