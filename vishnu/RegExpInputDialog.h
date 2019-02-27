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
