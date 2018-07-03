#ifndef VISHNU_PROPERTIESWIDGET_H
#define VISHNU_PROPERTIESWIDGET_H

#include <string>
#include <QTableWidgetItem>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <sp1common/sp1common.h>

#include "../Enums.hpp"

namespace vishnu
{

  class PropertiesWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesWidget( const std::string& name, const bool& use,
      const bool& primaryKey, const sp1common::DataType& dataType,
      const AxisType& axisType, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    void setName( const std::string& name );

    bool getUse( void ) const;
    void setUse( const bool& use );

    bool getPrimaryKey( void ) const;
    void setPrimaryKey( const bool& primaryKey );

    void fillDataTypes( void );
    sp1common::DataType getDataType( void ) const;
    void setDataType( const sp1common::DataType& dataType );

    void fillAxisTypes( void );
    AxisType getAxisType( void ) const;
    void setAxisType( const AxisType& axisType );

    QWidget* getWidget( const int& index );

  private slots:

    void useChanged( bool state );

    void primaryKeyChanged( bool state );

  private:

    QLabel* _name = nullptr;
    QCheckBox* _use = nullptr;
    QCheckBox* _primaryKey = nullptr;
    QComboBox* _dataType = nullptr;
    QComboBox* _axisType = nullptr;
  };

}

#endif
