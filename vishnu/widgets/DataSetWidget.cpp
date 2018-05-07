#include "DataSetWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QFocusEvent>
#include <QMessageBox>

#include <sstream>
#include <iostream>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{

  DataSetWidget::DataSetWidget( const std::string& name,
    const std::string& path, QWidget* /*parent*/ )
  {
    QHBoxLayout *hLayout = new QHBoxLayout( this );

    //From left to right
    //DataSet image
    QLabel* dataSetImage = new QLabel( );
    dataSetImage->setStyleSheet("width: 32px; height: 32px;");
    //dataSetImage->setScaledContents( true );
    QPixmap dataSetPixmap( ":/icons/dataSet.png" );
    dataSetImage->setPixmap( dataSetPixmap );

    QVBoxLayout *vLayout1 = new QVBoxLayout( );
    vLayout1->addWidget( dataSetImage, 0, 0);
    hLayout->addLayout( vLayout1, 0);
    hLayout->addSpacing(30);

    //Name and path
    setName( name );
    QObject::connect( _name, SIGNAL( clicked() ), this,
      SLOT( clickName( ) ) );
    setPath( path );

    QVBoxLayout *vLayout2 = new QVBoxLayout( );
    vLayout2->addWidget( _name, 0, 0);
    vLayout2->addWidget( _path, 1, 0);
    hLayout->addLayout( vLayout2, 1);
    hLayout->addSpacing(30);

    //DataSet CheckBox
    _checkbox.setChecked( true ); //default checked
    std::stringstream checkBoxStyleSheet;
    checkBoxStyleSheet << "QCheckBox::indicator { width: 32px; height: 32px; }"
      << "QCheckBox::indicator:checked { background-image:url(:/icons/checked.png); }"
      << "QCheckBox::indicator:unchecked { background-image:url(:/icons/unchecked.png); }";
    _checkbox.setStyleSheet( QString::fromStdString( checkBoxStyleSheet.str( ) ) );

    QVBoxLayout *vLayout3 = new QVBoxLayout( );
    vLayout3->addWidget( &_checkbox, 0, 0);
    hLayout->addLayout( vLayout3, 0);
    hLayout->addSpacing(30);

    //DataSet remove image
    QLabel* removeDataSetImage = new QLabel( );
    removeDataSetImage->setStyleSheet("width: 32px; height: 32px;");
    //removeDataSetImage->setScaledContents( true );
    QPixmap removeDataSetPixmap( ":/icons/close.png" );
    removeDataSetImage->setPixmap( removeDataSetPixmap );
    _remove = new QPushButton( );
    _remove->setIcon( QIcon( ":/icons/close.png" ) );
    QObject::connect( _remove, SIGNAL( clicked() ), this,
      SLOT( clickRemove( ) ) );

    QVBoxLayout *vLayout4 = new QVBoxLayout( );
    vLayout4->addWidget( _remove, 0, 0);
    hLayout->addLayout( vLayout4, 0);

    QPalette pal(palette());
    //pal.setColor(QPalette:, Qt::black);
    pal.setColor(QPalette::Base, QColor( "#c3e6fc" ));
    setAutoFillBackground(true);
    setPalette(pal);
    show();

    setLayout( hLayout );
    //setStyleSheet("border: 1px solid red");

    _headers = sp1common::Files::readCsvHeaders( path );
  }

  std::string DataSetWidget::getName( ) const
  {
    return _name->text( ).toStdString( );
  }

  void DataSetWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new sp1common::ClickableLabel( );
    }
    _name->setText( QString::fromStdString( name ) );
    _name->setStyleSheet("font-weight: bold; font-size: 20px");
  }

  std::string DataSetWidget::getPath( ) const
  {
    return _path->text( ).toStdString( );
  }

  void DataSetWidget::setPath( const std::string& path )
  {
    if ( _path == nullptr)
    {
      _path = new QLabel( );
    }
    _path->setText( QString::fromStdString( path ) );
  }

  bool DataSetWidget::getChecked( ) const
  {
    return _checkbox.isChecked( );
  }

  void DataSetWidget::setChecked( const bool& checked )
  {
    _checkbox.setChecked( checked );
  }

  void DataSetWidget::clickRemove( void )
  {
    _listWidgetItem->listWidget()->setCurrentItem( _listWidgetItem );
    emit removeSelected();
  }

  void DataSetWidget::clickName( void )
  {
    std::string tempName = _name->text( ).toStdString( );
    std::string name;

    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      QString dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name", QString::fromStdString( tempName ),
        regularExpression, &validName);
      if ( !validName )
      {
        return;
      }

      name = dataSetName.toStdString( );

      //Check if name doesn't exist
      notUsedName = true;
      QListWidget* listWidget = _listWidgetItem->listWidget( );
      for(int i = 0; i < listWidget->count(); ++i)
      {
        QListWidgetItem* item = listWidget->item( i );
        DataSetWidget* widget = static_cast< DataSetWidget* >(
          listWidget->itemWidget( item ) );
        if ( ( widget->getName( ) == name )
          && ( widget != this ) )
        {
          QMessageBox::warning( this, APPLICATION_NAME,
            tr("Invalid name. This name already exists.") );
          notUsedName = false;
        }
      }

    } while( !notUsedName );
    _name->setText( QString::fromStdString( name ) );
  }

  void DataSetWidget::clickPath( void )
  {
    emit updatePath( );
  }

  void DataSetWidget::clickCheckBox( void )
  {
    emit updateCheckBox( );
  }

  QListWidgetItem* DataSetWidget::getListWidgetItem() const
  {
    return _listWidgetItem;
  }

  void DataSetWidget::setListWidgetItem( QListWidgetItem* listWidgetItem )
  {
    _listWidgetItem = listWidgetItem;
  }

  std::vector< std::string > DataSetWidget::getHeaders( )
  {
    return _headers;
  }

}
