/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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
