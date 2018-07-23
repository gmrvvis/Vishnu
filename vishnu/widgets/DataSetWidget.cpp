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

  DataSetWidget::DataSetWidget( const std::string& path, QWidget* /*parent*/ )
  {
    QHBoxLayout *hLayout = new QHBoxLayout( this );

    //DataSet image
    QLabel* dataSetImage = new QLabel( );
    dataSetImage->setStyleSheet( "width: 32px; height: 32px;" );
    QPixmap dataSetPixmap( ":/icons/csv.png" );
    dataSetImage->setPixmap( dataSetPixmap );

    QVBoxLayout *vLayout1 = new QVBoxLayout( );
    vLayout1->addWidget( dataSetImage, 0, 0 );
    hLayout->addLayout( vLayout1, 0 );
    hLayout->addSpacing( 30 );

    //Path
    setPath( path );
    QVBoxLayout *vLayout2 = new QVBoxLayout( );
    vLayout2->addWidget( _path, 0, 0 );
    hLayout->addLayout( vLayout2, 1 );
    hLayout->addSpacing( 30 );

    //DataSet remove image
    QLabel* removeDataSetImage = new QLabel( );
    removeDataSetImage->setStyleSheet( "width: 32px; height: 32px;" );
    //removeDataSetImage->setScaledContents( true );
    QPixmap removeDataSetPixmap( ":/icons/close.png" );
    removeDataSetImage->setPixmap( removeDataSetPixmap );
    _remove = new QPushButton( );
    _remove->setIcon( QIcon( ":/icons/close.png" ) );
    QObject::connect( _remove, SIGNAL( clicked( ) ), this,
      SLOT( clickRemove( ) ) );

    QVBoxLayout *vLayout3 = new QVBoxLayout( );
    vLayout3->addWidget( _remove, 0, 0 );
    hLayout->addLayout( vLayout3, 0 );

    QPalette pal( palette( ) );
    pal.setColor( QPalette::Base, QColor( "#c3e6fc" ) );
    setAutoFillBackground( true );
    setPalette( pal );
    show( );

    setLayout( hLayout );

    _headers = sp1common::Files::readCsvHeaders( path );
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

  void DataSetWidget::clickRemove( void )
  {
    _listWidgetItem->listWidget()->setCurrentItem( _listWidgetItem );
    emit removeSelected();
  }

  void DataSetWidget::clickPath( void )
  {
    emit updatePath( );
  }

  QListWidgetItem* DataSetWidget::getListWidgetItem() const
  {
    return _listWidgetItem;
  }

  void DataSetWidget::setListWidgetItem( QListWidgetItem* listWidgetItem )
  {
    _listWidgetItem = listWidgetItem;
  }

  std::vector< std::string > DataSetWidget::getHeaders( ) const
  {
    return _headers;
  }

}
