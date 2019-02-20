/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef VISHNU_REGEXPINPUTDIALOG_H
#define VISHNU_REGEXPINPUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QRegularExpressionValidator>

namespace vishnu
{

  class RegExpInputDialog : public QDialog
  {

  Q_OBJECT

  public:

    explicit RegExpInputDialog( QWidget* parent = 0, Qt::WindowFlags = 0 );

    void setTitle( const QString& title );
    void setLabelText( const QString& label );
    void setText( const QString& text );
    void setRegularExpression( const QRegularExpression& regularExpression );

    QString getLabelText( void );
    QString getText( void );

    static QString getText( QWidget* parent, const QString& title,
      const QString& label, const QString& text,
      const QRegularExpression &regularExpression, bool* ok,
      Qt::WindowFlags flags = 0 );

  signals:

  private slots:

    void checkValid( const QString& text );

  private:

    QLabel* _label;
    QLineEdit* _text;
    QDialogButtonBox* _buttonBox;
    QRegularExpression _regularExpression;
    QRegularExpressionValidator* _validator;

  };

}

#endif
