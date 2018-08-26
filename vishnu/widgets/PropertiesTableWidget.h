/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

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

namespace vishnu
{

  class PropertiesTableWidget;
  typedef std::shared_ptr< PropertiesTableWidget > PropertiesTableWidgetPtr;

  class PropertiesTableWidget : public QTableWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesTableWidget( QWidget* parent = Q_NULLPTR );

    void checkPrimaryKeys( const std::vector< std::string >& commonProperties );

    void addProperties( const sp1common::Properties& properties,
      const sp1common::PropertyGroupsPtr& propertyGroups );

    void removeProperties( const std::vector< std::string >& properties );

    //Properties getProperties( void );
    sp1common::DataSetsPtr getDataSets( void );

  protected:

    void dragEnterEvent( QDragEnterEvent* event );

    void dragMoveEvent( QDragMoveEvent* event );

    void dragLeaveEvent( QDragLeaveEvent* event );

    void dropEvent( QDropEvent* event );

  private slots:

      void axisTypeChanged( QString text );

      void dataCategoryChanged( QString text );

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
