/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_PROPERTIESWIDGET_H
#define VISHNU_PROPERTIESWIDGET_H

#include <string>
#include <QTableWidgetItem>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <sp1common/sp1common.h>

namespace vishnu
{

  class PropertiesWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesWidget( const std::string& name, const bool& use,
      const bool& primaryKey, const sp1common::DataCategory& dataCategory,
      const sp1common::AxisType& axisType, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    void setName( const std::string& name );

    bool getUse( void ) const;
    void setUse( const bool& use );

    bool getPrimaryKey( void ) const;
    void setPrimaryKey( const bool& primaryKey );

    void fillDataCategories( void );
    sp1common::DataCategory getDataCategory( void ) const;
    void setDataCategory( const sp1common::DataCategory& dataCategory );

    void fillAxisTypes( void );
    sp1common::AxisType getAxisType( void ) const;
    void setAxisType( const sp1common::AxisType& axisType );

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
