#include "UserDataSetWidget.h"

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

  UserDataSetWidget::UserDataSetWidget( const std::string& name,
    const std::string& csvPath, const std::string& xmlPath,
    const bool& selected, QWidget* /*parent*/ )
  {
    QHBoxLayout *hLayout = new QHBoxLayout( this );

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

    //Name, paths and selected checkbox
    setName( name );
    QObject::connect( _name, SIGNAL( clicked() ), this,
      SLOT( clickName( ) ) );
    setCsvPath( csvPath );
    setXmlPath( xmlPath );
    setSelected( selected );

    QVBoxLayout *vLayout2 = new QVBoxLayout( );
    vLayout2->addWidget( _name, 0, 0);
    vLayout2->addWidget( _csvPath, 1, 0);
    vLayout2->addWidget( _xmlPath, 2, 0);
    hLayout->addLayout( vLayout2, 1);
    hLayout->addSpacing(30);

    //DataSet CheckBox
    _selected.setChecked( true ); //default checked
    std::stringstream checkBoxStyleSheet;
    checkBoxStyleSheet << "QCheckBox::indicator { width: 32px; height: 32px; }"
      << "QCheckBox::indicator:checked { background-image:url(:/icons/checked.png); }"
      << "QCheckBox::indicator:unchecked { background-image:url(:/icons/unchecked.png); }";
    _selected.setStyleSheet( QString::fromStdString( checkBoxStyleSheet.str( ) ) );

    QVBoxLayout *vLayout3 = new QVBoxLayout( );
    vLayout3->addWidget( &_selected, 0, 0);
    hLayout->addLayout( vLayout3, 0);
    hLayout->addSpacing(30);

    //DataSet remove image
    QLabel* removeDataSetImage = new QLabel( );
    removeDataSetImage->setStyleSheet("width: 32px; height: 32px;");
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
    pal.setColor(QPalette::Base, QColor( "#c3e6fc" ));
    setAutoFillBackground(true);
    setPalette(pal);
    show();

    setLayout( hLayout );
  }

  std::string UserDataSetWidget::getName( ) const
  {
    return _name->text( ).toStdString( );
  }

  void UserDataSetWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new sp1common::ClickableLabel( );
    }
    _name->setText( QString::fromStdString( name ) );
    _name->setStyleSheet("font-weight: bold; font-size: 20px");
  }

  std::string UserDataSetWidget::getCsvPath( ) const
  {
    return _csvPath->text( ).toStdString( );
  }

  void UserDataSetWidget::setCsvPath( const std::string& csvPath )
  {
    if ( _csvPath == nullptr)
    {
      _csvPath = new QLabel( );
    }
    _csvPath->setText( QString::fromStdString( csvPath ) );
  }

  std::string UserDataSetWidget::getXmlPath( ) const
  {
    return _xmlPath->text( ).toStdString( );
  }

  void UserDataSetWidget::setXmlPath( const std::string& xmlPath )
  {
    if ( _xmlPath == nullptr)
    {
      _xmlPath = new QLabel( );
    }
    _xmlPath->setText( QString::fromStdString( xmlPath ) );
  }

  bool UserDataSetWidget::getSelected( ) const
  {
    return _selected.isChecked( );
  }

  void UserDataSetWidget::setSelected( const bool& selected )
  {
    _selected.setChecked( selected );
  }

  void UserDataSetWidget::clickRemove( void )
  {
    _listWidgetItem->listWidget()->setCurrentItem( _listWidgetItem );
    emit removeSelected();
  }

  void UserDataSetWidget::clickName( void )
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
        UserDataSetWidget* widget = static_cast< UserDataSetWidget* >(
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

  void UserDataSetWidget::clickCheckBox( void )
  {
    emit updateCheckBox( );
  }

  QListWidgetItem* UserDataSetWidget::getListWidgetItem() const
  {
    return _listWidgetItem;
  }

  void UserDataSetWidget::setListWidgetItem( QListWidgetItem* listWidgetItem )
  {
    _listWidgetItem = listWidgetItem;
  }

}
