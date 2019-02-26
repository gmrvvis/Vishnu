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

#ifndef VISHNU_USERDATASETWIDGET_H
#define VISHNU_USERDATASETWIDGET_H

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <string>
#include <vector>
#include <memory>

#include <vishnucommon/vishnucommon.h>

namespace vishnu
{

  class UserDataSetWidget;
  using UserDataSetWidgetPtr = UserDataSetWidget*;
  using UserDataSetWidgets = std::vector< UserDataSetWidgetPtr >;

  class UserDataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetWidget( const std::string& name,
      const std::string& path, const std::string& csvFilename,
      const std::string& jsonFilename, const std::string& xmlFilename,
      const bool& selected, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    std::string getPath( void ) const;
    std::string getCsvFilename( void ) const;
    std::string getCsvPath( void ) const;
    std::string getJsonFilename( void ) const;
    std::string getJsonPath( void ) const;
    std::string getXmlFilename( void ) const;
    std::string getXmlPath( void ) const;

    QCheckBox* getCheckBox( void ) const;
    bool getSelected( void ) const;
    void setSelected( const bool& selected );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

  signals:

    void signalCheckSelected( bool checked );
    void signalRemoveSelected( void );
    void updateName( void );
    void updateCheckBox( void );

  private slots:

    void clickName( void );
    void slotCheck( bool check );
    void slotRemove( void );

  private:

    vishnucommon::ClickableLabel* _name = nullptr;
    std::string _path;
    std::string _csvFilename;
    QLabel* _csvPath = nullptr;
    std::string _jsonFilename;
    QLabel* _jsonPath = nullptr;
    std::string _xmlFilename;
    QLabel* _xmlPath = nullptr;
    QCheckBox* _selected = nullptr;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;

    void setName( const std::string& name );
    void setCsvPath( const std::string& path, const std::string& csvFilename );
    void setJsonPath( const std::string& path,
      const std::string& jsonFilename );
    void setXmlPath( const std::string& path, const std::string& xmlFilename );

  };

}

#endif
