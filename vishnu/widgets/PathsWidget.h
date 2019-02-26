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

#ifndef VISHNU_PATHSWIDGET_H
#define VISHNU_PATHSWIDGET_H

#include <QPushButton>
#include <QLineEdit>

#include <string>
#include <memory>

#include "../model/UserDataSet.h"

#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class PathsWidget;
  using PathsWidgetPtr = std::shared_ptr< PathsWidget >;

  class PathsWidget : public QWidget
  {

    Q_OBJECT

    public:

      PathsWidget( void );
      ~PathsWidget( void );

      std::string getName( void ) const;
      std::string getPath( void ) const;
      std::string getJsonFilename( void ) const;
      std::string getCsvFilename( void ) const;
      std::string getXmlFilename( void ) const;

      UserDataSetPtr getUserDataSet( void );

    private slots:
      void slotBrowsePath( void );

    private:
      QLineEdit* _name = nullptr;
      QLineEdit* _path = nullptr;
      QPushButton* _browsePath = nullptr;
      QLineEdit* _csvFilename = nullptr;
      QLineEdit* _jsonFilename = nullptr;
      QLineEdit* _xmlFilename = nullptr;

      void setName( const std::string& name );
      void setPath( const std::string& path );
      void setBrowsePath( void );
      void setCsvFilename( const std::string& csvFilename );
      void setJsonFilename( const std::string& jsonFilename );
      void setXmlFilename( const std::string& xmlFilename );

  };

}

#endif
