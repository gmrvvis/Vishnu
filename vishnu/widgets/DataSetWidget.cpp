#include "DataSetWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <sstream>

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
    //QObject::connect( actionRemoveDataSet, SIGNAL( triggered() ), this,
      //SLOT( removeDataSetItem( ) ) );

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
  }

  std::string DataSetWidget::getName( ) const
  {
    return _name->text( ).toStdString( );
  }

  void DataSetWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new QLabel( );
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

  QPushButton* DataSetWidget::getPushButton()
  {
    return _remove;
  }

}
