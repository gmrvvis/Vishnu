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

#include <vishnucommon/vishnucommon.h>
#include "PropertiesWidget.h"

namespace vishnu
{

  class PropertiesTableWidget;
  using PropertiesTableWidgetPtr = std::shared_ptr< PropertiesTableWidget >;

  class PropertiesTableWidget : public QTableWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesTableWidget( QWidget* parent = Q_NULLPTR );

    void checkPrimaryKeys( const std::vector< std::string >& commonProperties );

    void addProperties( const vishnucommon::Properties& properties,
      const vishnucommon::PropertyGroupsPtr& propertyGroups );

    void removeProperties( const std::vector< std::string >& properties );

    vishnucommon::DataSetsPtr getDataSets( void );

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
        const vishnucommon::AxisType& axisType,
        const std::vector< vishnucommon::AxisType >& toExclude );

      void changeToNoneOrXOrYOrZ(
        const std::vector< vishnucommon::AxisType >& selectedAxis );

      void changeToXYZ(
        const std::vector< vishnucommon::AxisType >& selectedAxis );

      PropertiesWidget* getRow( const int& index );

      int _sourceIndex;

  };

}

#endif
