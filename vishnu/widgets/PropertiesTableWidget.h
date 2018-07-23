#ifndef VISHNU_PROPERTIESTABLEWIDGET_H
#define VISHNU_PROPERTIESTABLEWIDGET_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>

#include <sp1common/sp1common.h>
#include "PropertiesWidget.h"
#include "../Property.h"

namespace vishnu
{

  typedef std::shared_ptr< Property > PropertyPtr;
  typedef std::vector< PropertyPtr > Properties;

  class PropertiesTableWidget : public QTableWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesTableWidget( QWidget* parent = Q_NULLPTR );

    void checkPrimaryKeys( const std::vector< std::string >& commonProperties );

    void addProperties( const std::vector< std::string >& properties );

    void removeProperties( const std::vector< std::string >& properties );

    Properties getProperties( );

  private slots:

      void axisTypeChanged( QString text );

  private:

      bool _checkingProperty;

      void refillAxisTypeComboBox( QComboBox* combo,
        const sp1common::AxisType& axisType,
        const std::vector< sp1common::AxisType >& toExclude );

      void changeToNoneOrXOrYOrZ(
        const std::vector< sp1common::AxisType >& selectedAxis );

      void changeToXYZ(
        const std::vector< sp1common::AxisType >& selectedAxis );

  };

}

#endif
