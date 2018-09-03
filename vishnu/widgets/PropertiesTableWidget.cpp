/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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
    const sp1common::Properties& properties,
    const sp1common::PropertyGroupsPtr& propertyGroups )
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
        std::vector< std::string > primaryKeys =
          propertyGroups->getPrimaryKeys( );
        std::vector< std::string > nonPrimaryKeys =
          propertyGroups->getNonPrimaryKeys( );
        std::vector< std::string > axes = propertyGroups->getAxes( );
        bool isPrimaryKey = false;
        bool inUse = false;
        sp1common::AxisType axis = sp1common::AxisType::None;
        if ( sp1common::Vectors::find( primaryKeys, propertyName ) != -1 )
        {
          isPrimaryKey = true;
          inUse = true;
        }
        else if ( sp1common::Vectors::find( nonPrimaryKeys, propertyName )
          != -1 )
        {
          inUse = true;
        }
        int axisPos = sp1common::Vectors::find( axes, propertyName );
        if ( axisPos != -1 )
        {
          if ( axes.size( ) > 1 )
          {
            switch( axisPos )
            {
              case 0:
                axis = sp1common::AxisType::X;
                break;
              case 1:
                axis = sp1common::AxisType::Y;
                break;
              case 2:
                axis = sp1common::AxisType::Z;
                break;
              default:
                sp1common::Error::throwError(
                  sp1common::Error::ErrorType::Error, "Invalid Axis", true );
                break;
            }
          }
          else
          {
            axis = sp1common::AxisType::XYZ;
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
    const sp1common::AxisType& axisType,
    const std::vector< sp1common::AxisType >& toExclude )
  {
    combo->clear( );
    for ( const auto& at : axisTypesToVector( toExclude ) )
    {
      combo->addItem( QString::fromStdString( at ) );
    }
    combo->setCurrentText( QString::fromStdString( toString( axisType ) ) );
  }

  void PropertiesTableWidget::changeToNoneOrXOrYOrZ(
    const std::vector< sp1common::AxisType >& selectedAxis )
  {
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      sp1common::AxisType axisType = sp1common::toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< sp1common::AxisType > toExclude = selectedAxis;
      sp1common::Vectors::remove( toExclude, axisType );

      if ( ( ( sp1common::Vectors::find( toExclude, sp1common::AxisType::X ) != -1 )
        || ( sp1common::Vectors::find( toExclude, sp1common::AxisType::Y ) != -1 )
        || ( sp1common::Vectors::find( toExclude, sp1common::AxisType::Z ) != -1 ) )
        && ( sp1common::Vectors::find( toExclude, sp1common::AxisType::XYZ ) == -1 ) )
      {
        toExclude.push_back( sp1common::AxisType::XYZ );
      }

      refillAxisTypeComboBox( axisCB, axisType, toExclude );
    }
  }

  void PropertiesTableWidget::changeToXYZ(
    const std::vector< sp1common::AxisType >& selectedAxis )
  {
    //Loop over properties removing X, Y, Z and XYZ options
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      sp1common::AxisType axisType = sp1common::toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< sp1common::AxisType > toExclude = selectedAxis;
      sp1common::Vectors::remove( toExclude, axisType );

      if ( sp1common::Vectors::find( toExclude, sp1common::AxisType::XYZ )
        != -1 )
      {
        toExclude.push_back( sp1common::AxisType::X );
        toExclude.push_back( sp1common::AxisType::Y );
        toExclude.push_back( sp1common::AxisType::Z );
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
      std::vector< sp1common::AxisType > selectedAxis;
      for (int i = 0; i < rowCount( ); ++i )
      {
        QComboBox* cb = static_cast< QComboBox* >( cellWidget( i, 4 ) );
        sp1common::AxisType at = sp1common::toAxisType(
          cb->currentText( ).toStdString( ) );
        if ( at != sp1common::AxisType::None )
        {
          selectedAxis.push_back( at );
        }
      }

      /* Second:
       Get sender combobox and loop over all combos excluding some results
      */
      sp1common::AxisType senderAxisType =
        sp1common::toAxisType( text.toStdString( ) );
      switch ( senderAxisType )
      {
        case sp1common::AxisType::None:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case sp1common::AxisType::X:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case sp1common::AxisType::Y:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case sp1common::AxisType::Z:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case sp1common::AxisType::XYZ:
          changeToXYZ( selectedAxis );
          break;
      }
      _checkingProperty = false;
    }
  }

  void PropertiesTableWidget::dataCategoryChanged( QString text )
  {
    if ( sp1common::toDataCategory( text.toStdString( ) )
      == sp1common::DataCategory::Geometric )
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
              if ( sp1common::toDataCategory( cb->currentText( ).toStdString( ) )
                == sp1common::DataCategory::Geometric )
              {
                cb->setCurrentText( QString::fromStdString( toString(
                  sp1common::DataCategory::Undefined ) ) );
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

  sp1common::DataSetsPtr PropertiesTableWidget::getDataSets( void )
  {
    sp1common::DataSetsPtr dataSets( new sp1common::DataSets( ) );
    sp1common::DataSetPtr dataSet( new sp1common::DataSet( ) );
    sp1common::PropertyGroupsPtr propertyGroups(
      new sp1common::PropertyGroups( ) );
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
      sp1common::DataCategory dataCategory = sp1common::toDataCategory(
        dataTypeComboBox->currentText( ).toStdString( ) );
      QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
        row, 4 ) );
      sp1common::AxisType axisType = sp1common::toAxisType(
        axisTypeComboBox->currentText( ).toStdString( ) );

      if ( use )
      {
        if ( primaryKey )
        {
          propertyGroups->addPrimaryKey( name );
        }
        else
        {
          propertyGroups->addNonPrimaryKey( name );
        }
        dataSet->addProperty( sp1common::PropertyPtr(
          new sp1common::Property( name, dataCategory,
          sp1common::DataStructureType::None ) ) );
      }

      switch( axisType )
      {
        case sp1common::AxisType::X:
          xAxis = name;
          break;
        case sp1common::AxisType::Y:
          yAxis = name;
          break;
        case sp1common::AxisType::Z:
          zAxis = name;
          break;
        case sp1common::AxisType::XYZ:
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
      sp1common::Error::throwError( sp1common::Error::ErrorType::Error,
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
    sp1common::DataCategory dataCategory = sp1common::toDataCategory(
      dataCategoryComboBox->currentText( ).toStdString( ) );
    QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
      index, 4 ) );
    sp1common::AxisType axisType = sp1common::toAxisType(
      axisTypeComboBox->currentText( ).toStdString( ) );

    PropertiesWidget* propertiesWidget = new PropertiesWidget( name, use,
      primaryKey, dataCategory, axisType );

    return propertiesWidget;
  }

}
