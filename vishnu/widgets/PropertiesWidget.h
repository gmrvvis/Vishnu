/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
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

#ifndef VISHNU_PROPERTIESWIDGET_H
#define VISHNU_PROPERTIESWIDGET_H

#include <string>
#include <QTableWidgetItem>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class PropertiesWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesWidget( const std::string& name, const bool& use,
      const bool& primaryKey, const vishnucommon::DataCategory& dataCategory,
      const vishnucommon::AxisType& axisType, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    void setName( const std::string& name );

    bool getUse( void ) const;
    void setUse( const bool& use );

    bool getPrimaryKey( void ) const;
    void setPrimaryKey( const bool& primaryKey );

    void fillDataCategories( void );
    vishnucommon::DataCategory getDataCategory( void ) const;
    void setDataCategory( const vishnucommon::DataCategory& dataCategory );

    void fillAxisTypes( void );
    vishnucommon::AxisType getAxisType( void ) const;
    void setAxisType( const vishnucommon::AxisType& axisType );

    QWidget* getWidget( const int& index );

  private slots:

    void useChanged( bool state );

    void primaryKeyChanged( bool state );

  private:

    QLabel* _name = nullptr;
    QCheckBox* _use = nullptr;
    QCheckBox* _primaryKey = nullptr;
    QComboBox* _dataCategory = nullptr;
    QComboBox* _axisType = nullptr;
  };

}

#endif
