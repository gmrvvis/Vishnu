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
  {
    setColumnCount( 5 );
    setColumnWidth( 0, 220 );
    setColumnWidth( 1, 80 );
    setColumnWidth( 2, 150 );
    setColumnWidth( 3, 100 );
    setColumnWidth( 4, 100 );

    horizontalHeader()->setStretchLastSection( true );
    QStringList headers;
    headers << "Property name" << "Use" << "Primary Key" << "Data type"
      << "Axis type";
    setHorizontalHeaderLabels( headers );
    setSortingEnabled( false );
    verticalHeader( )->setVisible( false );
    setAcceptDrops( true );
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
    const std::vector< std::string >& properties )
  {
    for ( const auto& property : properties )
    {
      std::vector< std::string > tableProperties;
      for (int i = 0; i < rowCount(); ++i )
      {
        std::string propertyName = static_cast< QLabel* >( cellWidget( i, 0 )
          )->text( ).toStdString( );

        tableProperties.push_back( propertyName );
      }

      if ( std::find( tableProperties.begin( ), tableProperties.end( ),
        property ) == tableProperties.end( ) )
      {
        int row = this->rowCount( );
        int columns = columnCount( );

        PropertiesWidget* propertiesWidget = new PropertiesWidget( property,
          true, false, sp1common::DataType::Undefined,
          sp1common::AxisType::None );

        insertRow(row);
        for( int column = 0; column < columns; ++column )
        {
          setItem( row, column, new QTableWidgetItem( ) );
          setCellWidget( row, column, propertiesWidget->getWidget( column ) );
        }

        //Axis Type
        QObject::connect( propertiesWidget->getWidget( 4 ),
          SIGNAL( currentIndexChanged( QString ) ), this,
          SLOT( axisTypeChanged( QString ) ) );

        //DataType
        QObject::connect( propertiesWidget->getWidget( 3 ),
          SIGNAL( currentIndexChanged( QString ) ), this,
          SLOT( dataTypeChanged( QString ) ) );
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

      if ( sp1common::Vectors::find( toExclude, sp1common::AxisType::XYZ ) != -1 )
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

  void PropertiesTableWidget::dataTypeChanged( QString text )
  {
    if ( sp1common::toDataType( text.toStdString( ) )
      == sp1common::DataType::Geometric )
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
              if ( sp1common::toDataType( cb->currentText( ).toStdString( ) )
                == sp1common::DataType::Geometric )
              {
                cb->setCurrentText( QString::fromStdString( toString(
                  sp1common::DataType::Undefined ) ) );
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
      sp1common::DataType dataType = sp1common::toDataType(
        dataTypeComboBox->currentText( ).toStdString( ) );
      QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
        row, 4 ) );
      sp1common::AxisType axisType = sp1common::toAxisType(
        axisTypeComboBox->currentText( ).toStdString( ) );

      if ( primaryKey )
      {
        propertyGroups->addPrimaryKey( name );
      }

      if ( use )
      {
        propertyGroups->addUse( name );

        dataSet->addProperty( sp1common::PropertyPtr(
          new sp1common::Property( name, dataType,
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

  /*Properties PropertiesTableWidget::getProperties( void )
  {
    Properties properties;

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
      sp1common::DataType dataType = sp1common::toDataType(
        dataTypeComboBox->currentText( ).toStdString( ) );
      QComboBox* axisTypeComboBox = static_cast< QComboBox* >( cellWidget(
        row, 4 ) );
      sp1common::AxisType axisType = sp1common::toAxisType(
        axisTypeComboBox->currentText( ).toStdString( ) );

      PropertyPtr property( new Property( name, use, primaryKey, dataType, 
        axisType ) );
      properties.emplace_back( property );
    }

    return properties;
  }*/
}
