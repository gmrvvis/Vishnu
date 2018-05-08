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

  void PropertiesTableWidget::addProperties(
    const std::vector< std::string >& properties )
  {
    for ( const auto& property : properties )
    {
      std::vector< std::string > tableProperties;
      for (int i = 0; i < rowCount(); ++i )
      {
        tableProperties.push_back( item( i, 0 )->text( ).toStdString( ) );
      }

      if ( std::find(tableProperties.begin( ), tableProperties.end( ),
        property ) == tableProperties.end( ) )
      {
        int row = this->rowCount();
        int columns = columnCount();

        QWidgetList* widgetList = new QWidgetList();
        widgetList->push_back( new QLabel( QString::fromStdString( property ) ) );
        QCheckBox* usePropertyCheckBox = new QCheckBox( );
        usePropertyCheckBox->setChecked(true);
        widgetList->push_back( usePropertyCheckBox );
        widgetList->push_back( new QCheckBox( ) );
        QComboBox* dataTypeComboBox = new QComboBox( );
        for (const auto& dataType : sp1common::dataTypeToVector( ) )
        {
           dataTypeComboBox->addItem( QString::fromStdString( dataType ) );
        }
        widgetList->push_back( dataTypeComboBox );

        insertRow(row);
        for( int column = 0; column < columns; ++column )
        {
          setItem( row, column, new QTableWidgetItem( ) );
          setCellWidget( row, column, widgetList->at( column ) );
        }
        //_ui->propertiesTableWidget->show();
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
        dataTypeComboBox->itemData( dataTypeComboBox->currentIndex( )
      ).toString( ).toStdString( ) );

      PropertyPtr property( new Property( name, use, primaryKey, dataType) );
      properties.push_back( property );
    }

    return properties;
  }

}
