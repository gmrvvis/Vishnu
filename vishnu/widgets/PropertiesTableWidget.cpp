#include "PropertiesTableWidget.h"

#include <algorithm>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QHeaderView>

namespace vishnu
{
  PropertiesTableWidget::PropertiesTableWidget( QWidget* /*parent*/ )
  {
    setColumnCount( 4 );
    setColumnWidth( 0, 320 );
    setColumnWidth( 1, 80 );
    setColumnWidth( 2, 100 );
    setColumnWidth( 3, 150 );
    horizontalHeader()->setStretchLastSection( true );
    QStringList headers;
    headers << "Property name" << "Use" << "Primary Key" << "Data type";
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
        int row = this->rowCount();
        int columns = columnCount();

        PropertiesWidget* propertiesWidget = new PropertiesWidget( property,
          true, false, sp1common::DataType::Undefined );

        insertRow(row);
        for( int column = 0; column < columns; ++column )
        {
          setItem( row, column, new QTableWidgetItem( ) );
          setCellWidget( row, column, propertiesWidget->getWidget( column ) );
        }
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

      PropertyPtr property( new Property( name, use, primaryKey, dataType) );
      properties.push_back( property );
    }

    return properties;
  }
}
