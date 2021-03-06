/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    const std::string& path, const std::string& csvFilename,
    const std::string& jsonFilename, const std::string& xmlFilename,
    const bool& selected, QWidget* /*parent*/ )
      : _path( path )
      , _csvFilename( csvFilename )
      , _jsonFilename( jsonFilename )
      , _xmlFilename( xmlFilename )
  {
    QHBoxLayout *hLayout = new QHBoxLayout( this );

    //DataSet image
    QLabel* dataSetImage = new QLabel( );
    dataSetImage->setStyleSheet("width: 32px; height: 32px;");
    QPixmap dataSetPixmap( ":/icons/dataSet.png" );
    dataSetImage->setPixmap( dataSetPixmap );

    QVBoxLayout *vLayout1 = new QVBoxLayout( );
    vLayout1->addWidget( dataSetImage, 0, 0 );
    hLayout->addLayout( vLayout1, 0 );
    hLayout->addSpacing( 30 );

    //Name, paths and selected checkbox
    setName( name );
    QObject::connect( _name, SIGNAL( clicked() ), this,
      SLOT( clickName( ) ) );
    setCsvPath( path, csvFilename );
    setJsonPath( path, jsonFilename );
    setXmlPath( path, xmlFilename );
    setSelected( selected );

    QVBoxLayout *vLayout2 = new QVBoxLayout( );
    vLayout2->addWidget( _name, 0, 0 );
    hLayout->addLayout( vLayout2, 1 );
    hLayout->addSpacing( 30 );

    QVBoxLayout *vLayout3 = new QVBoxLayout( );
    vLayout3->addWidget( _csvPath, 1, 0 );
    vLayout3->addWidget( _jsonPath, 2, 0 );
    vLayout3->addWidget( _xmlPath, 3, 0 );
    hLayout->addLayout( vLayout3, 2 );
    hLayout->addSpacing( 30 );

    //DataSet CheckBox
    _selected->setStyleSheet( "QCheckBox::indicator { width: 32px; height: 32px; }"
      "QCheckBox::indicator:checked { background-image:url(:/icons/checked.png); }"
      "QCheckBox::indicator:unchecked { background-image:url(:/icons/unchecked.png); }"
    );
    QObject::connect( _selected, SIGNAL( toggled( bool ) ), this,
      SLOT( slotCheck( bool ) ) );

    QVBoxLayout *vLayout4 = new QVBoxLayout( );
    vLayout4->addWidget( _selected, 0, 0);
    hLayout->addLayout( vLayout4, 0);
    hLayout->addSpacing(30);

    //DataSet remove image
    QLabel* removeDataSetImage = new QLabel( );
    removeDataSetImage->setStyleSheet("width: 32px; height: 32px;");
    QPixmap removeDataSetPixmap( ":/icons/close.png" );
    removeDataSetImage->setPixmap( removeDataSetPixmap );
    _remove = new QPushButton( );
    _remove->setIcon( QIcon( ":/icons/close.png" ) );
    _remove->setStyleSheet( "QWidget:hover{background-color:#AAAAFF;}" );
    QObject::connect( _remove, SIGNAL( clicked( ) ), this,
      SLOT( slotRemove( ) ) );

    QVBoxLayout *vLayout5 = new QVBoxLayout( );
    vLayout5->addWidget( _remove, 0, 0);
    hLayout->addLayout( vLayout5, 0);

    /*QPalette pal(palette());
    pal.setColor(QPalette::Base, QColor( "#c3e6fc" ));
    setAutoFillBackground(true);
    setPalette(pal);*/

    setLayout( hLayout );

    show();
  }

  std::string UserDataSetWidget::getName( ) const
  {
    return _name->text( ).toStdString( );
  }

  void UserDataSetWidget::setName( const std::string& name )
  {
    if ( _name == nullptr)
    {
      _name = new vishnucommon::ClickableLabel( );
    }
    _name->setText( QString::fromStdString( name ) );
    _name->setStyleSheet("font-weight: bold; font-size: 20px");
  }

  std::string UserDataSetWidget::getPath( ) const
  {
    return _path;
  }

  std::string UserDataSetWidget::getCsvFilename( ) const
  {
    return _csvFilename;
  }

  std::string UserDataSetWidget::getCsvPath( ) const
  {
    return _csvPath->text( ).toStdString( );
  }

  void UserDataSetWidget::setCsvPath( const std::string& path,
    const std::string& csvFilename )
  {
    if ( _csvPath == nullptr)
    {
      _csvPath = new QLabel( );
    }
    _csvPath->setText( QString::fromStdString( path + std::string( "/" )
      + csvFilename ) );
  }

  std::string UserDataSetWidget::getJsonFilename( void ) const
  {
    return _jsonFilename;
  }

  std::string UserDataSetWidget::getJsonPath( void ) const
  {
    return _jsonPath->text( ).toStdString( );
  }

  void UserDataSetWidget::setJsonPath( const std::string& path,
    const std::string& jsonFilename )
  {
    if ( _jsonPath == nullptr)
    {
      _jsonPath = new QLabel( );
    }
    _jsonPath->setText( QString::fromStdString( path + std::string( "/" )
      + jsonFilename ) );
  }

  std::string UserDataSetWidget::getXmlFilename( ) const
  {
    return _xmlFilename;
  }

  std::string UserDataSetWidget::getXmlPath( ) const
  {
    return _xmlPath->text( ).toStdString( );
  }

  void UserDataSetWidget::setXmlPath( const std::string& path,
    const std::string& xmlFilename )
  {
    if ( _xmlPath == nullptr)
    {
      _xmlPath = new QLabel( );
    }
    _xmlPath->setText( QString::fromStdString( path + std::string( "/" )
      + xmlFilename ) );
  }

  QCheckBox* UserDataSetWidget::getCheckBox( void ) const
  {
    return _selected;
  }

  bool UserDataSetWidget::getSelected( ) const
  {
    return _selected->isChecked( );
  }

  void UserDataSetWidget::setSelected( const bool& selected )
  {
    if ( _selected == nullptr)
    {
      _selected = new QCheckBox( );
    }
    _selected->setChecked( selected );
  }

  void UserDataSetWidget::slotRemove( void )
  {
    _listWidgetItem->listWidget( )->setCurrentItem( _listWidgetItem );
    emit signalRemoveSelected( );
  }

  void UserDataSetWidget::slotCheck( bool check )
  {
    _listWidgetItem->listWidget( )->setCurrentItem( _listWidgetItem );
    emit signalCheckSelected( check );
  }

  void UserDataSetWidget::clickName( void )
  {
    QString tempName = _name->text( );
    QString name;

    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      QString dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name", tempName, regularExpression, &validName);
      if ( !validName )
      {
        return;
      }

      name = dataSetName;

      //Check if name doesn't exist
      notUsedName = true;
      QListWidget* listWidget = _listWidgetItem->listWidget( );
      for(int i = 0; i < listWidget->count(); ++i)
      {
        QListWidgetItem* item = listWidget->item( i );
        UserDataSetWidget* widget = static_cast< UserDataSetWidget* >(
          listWidget->itemWidget( item ) );
        if ( ( widget->getName( ) == name.toStdString( ) )
          && ( widget != this ) )
        {
          QMessageBox::warning( this, APPLICATION_NAME,
            tr("Invalid name. This name already exists.") );
          notUsedName = false;
        }
      }

    } while( !notUsedName );
    _name->setText( name );
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
