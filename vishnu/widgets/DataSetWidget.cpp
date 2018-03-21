#include "DataSetWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

namespace vishnu
{

  DataSetWidget::DataSetWidget( const std::string& name,
    const std::string& path, QWidget* /*parent*/ )
  {
    setName( name );
    setPath( path );

    QLabel* image = new QLabel( );
    image->setBackgroundRole( QPalette::Dark );
    //image->setScaledContents( true );
    QPixmap pixmap( ":/icons/addDataSet.png" );
    image->setPixmap( pixmap );

    QHBoxLayout *hLayout = new QHBoxLayout( this );
    QVBoxLayout *vLeftLayout = new QVBoxLayout( );
    vLeftLayout->addWidget( image, 0, 0);

    hLayout->addLayout( vLeftLayout, 0);
    //hLayout->addWidget( image, 0, 0);
    hLayout->addSpacing(30);

    QVBoxLayout *vLayout = new QVBoxLayout( );
    vLayout->addWidget( _name, 0, 0);
    vLayout->addWidget( _path, 1, 0);

    hLayout->addLayout( vLayout, 1);



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

}
