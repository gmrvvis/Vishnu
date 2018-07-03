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
    setColumnWidth( 2, 100 );
    setColumnWidth( 3, 150 );
    setColumnWidth( 4, 100 );
    horizontalHeader()->setStretchLastSection( true );
    QStringList headers;
    headers << "Property name" << "Use" << "Primary Key" << "Data type" << "Axis type";
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
          true, false, sp1common::DataType::Undefined, AxisType::None );

        insertRow(row);
        for( int column = 0; column < columns; ++column )
        {
          setItem( row, column, new QTableWidgetItem( ) );
          setCellWidget( row, column, propertiesWidget->getWidget( column ) );
        }

        QObject::connect( propertiesWidget->getWidget( 4 ),
          SIGNAL( currentIndexChanged( QString ) ), this,
          SLOT( axisTypeChanged( QString ) ) );
      }
    }
  }

  void PropertiesTableWidget::refillAxisTypeComboBox( QComboBox* combo,
    const AxisType& axisType, const std::vector< AxisType >& toExclude )
  {
    combo->clear( );
    for ( const auto& at : axisTypesToVector( toExclude ) )
    {
      combo->addItem( QString::fromStdString( at ) );
    }
    combo->setCurrentText( QString::fromStdString( toString( axisType ) ) );
  }

  void PropertiesTableWidget::changeToNoneOrXOrYOrZ(
    const std::vector< AxisType >& selectedAxis )
  {
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      AxisType axisType = toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< AxisType > toExclude = selectedAxis;
      sp1common::Vectors::remove( toExclude, axisType );

      if ( ( ( sp1common::Vectors::find( toExclude, AxisType::X ) != -1 )
        || ( sp1common::Vectors::find( toExclude, AxisType::Y ) != -1 )
        || ( sp1common::Vectors::find( toExclude, AxisType::Z ) != -1 ) )
        && ( sp1common::Vectors::find( toExclude, AxisType::XYZ ) == -1 ) )
      {
        toExclude.push_back( AxisType::XYZ );
      }

      refillAxisTypeComboBox( axisCB, axisType, toExclude );
    }
  }

  void PropertiesTableWidget::changeToXYZ(
    const std::vector< AxisType >& selectedAxis )
  {
    //Loop over properties removing X, Y, Z and XYZ options
    for (int i = 0; i < rowCount(); ++i )
    {
      QComboBox* axisCB = static_cast< QComboBox* >(
        cellWidget( i, 4 ) );
      AxisType axisType = toAxisType(
        axisCB->currentText( ).toStdString( ) );

      std::vector< AxisType > toExclude = selectedAxis;
      sp1common::Vectors::remove( toExclude, axisType );

      if ( sp1common::Vectors::find( toExclude, AxisType::XYZ ) != -1 )
      {
        toExclude.push_back( AxisType::X );
        toExclude.push_back( AxisType::Y );
        toExclude.push_back( AxisType::Z );
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
      std::vector< AxisType > selectedAxis;
      for (int i = 0; i < rowCount(); ++i )
      {
        QComboBox* cb = static_cast< QComboBox* >( cellWidget( i, 4 ) );
        AxisType at = toAxisType( cb->currentText( ).toStdString( ) );
        if ( at != AxisType::None )
        {
          selectedAxis.push_back( at );
        }
      }

      /* Second:
       Get sender combobox and loop over all combos excluding some results
      */
      AxisType senderAxisType = toAxisType( text.toStdString( ) );
      switch ( senderAxisType )
      {
        case AxisType::None:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case AxisType::X:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case AxisType::Y:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case AxisType::Z:
          changeToNoneOrXOrYOrZ( selectedAxis );
          break;
        case AxisType::XYZ:
          changeToXYZ( selectedAxis );
          break;
      }
      _checkingProperty = false;
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

  Properties PropertiesTableWidget::getProperties( )
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
      AxisType axisType = toAxisType( 
        axisTypeComboBox->currentText( ).toStdString( ) );

      PropertyPtr property( new Property( name, use, primaryKey, dataType, 
        axisType ) );
      properties.push_back( property );
    }

    return properties;
  }
}
