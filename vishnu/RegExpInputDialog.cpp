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

#include "RegExpInputDialog.h"

#include <QPushButton>
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>

namespace vishnu
{

  RegExpInputDialog::RegExpInputDialog( QWidget *parent, Qt::WindowFlags flags )
    : QDialog( parent )
  {
    if( flags!=0 ) setWindowFlags( flags );

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size( ),
            qApp->desktop( )->availableGeometry( )
        )
    );

    QVBoxLayout *l = new QVBoxLayout( this );

    _label=new QLabel( this );

    //_regularExpression=QRegularExpression("*");
    _validator = new QRegularExpressionValidator( _regularExpression );

    _text = new QLineEdit( this );
    _text->setValidator( _validator );
    QObject::connect( _text, SIGNAL( textChanged( QString ) ), this,
      SLOT( checkValid( QString ) ) );

    _buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok |
      QDialogButtonBox::Cancel, Qt::Horizontal, this );
    QObject::connect( _buttonBox, SIGNAL( accepted( ) ), this,
      SLOT( accept( ) ) );
    QObject::connect( _buttonBox, SIGNAL( rejected( ) ), this,
      SLOT( reject( ) ) );

    l->addWidget( _label );
    l->addWidget( _text );
    l->addWidget( _buttonBox );
  }

  void RegExpInputDialog::setTitle( const QString& title )
  {
    setWindowTitle( title );
  }

  void RegExpInputDialog::setLabelText( const QString& label )
  {
    _label->setText( label );
  }

  void RegExpInputDialog::setText( const QString& text )
  {
    _text->setText(text);
  }

  void RegExpInputDialog::setRegularExpression(
    const QRegularExpression &regularExpression )
  {
    _validator->setRegularExpression( regularExpression );
    checkValid( _text->text( ) );
  }

  QString RegExpInputDialog::getLabelText( )
  {
    return _label->text( );
  }

  QString RegExpInputDialog::getText( )
  {
    return _text->text( );
  }

  void RegExpInputDialog::checkValid( const QString &text )
  {
    QString txt = QString( text );
    int pos = 0;
    bool valid=_validator->validate( txt, pos ) == QRegExpValidator::Acceptable;
    _buttonBox->button( QDialogButtonBox::Ok )->setEnabled( valid );
  }

  QString RegExpInputDialog::getText( QWidget *parent, const QString& title,
    const QString& label, const QString& text,
    const QRegularExpression& regularExpression, bool* ok,
    Qt::WindowFlags flags)
  {
    RegExpInputDialog *r = new RegExpInputDialog( parent, flags );
    r->setTitle( title );
    r->setLabelText( label );
    r->setText( text );
    r->setRegularExpression( regularExpression );
    *ok = r->exec( )==QDialog::Accepted;
    if( *ok ) return r->getText( );
    else return QString( );
  }

}
