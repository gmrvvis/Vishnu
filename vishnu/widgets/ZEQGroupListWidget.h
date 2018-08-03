/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_ZEQGROUPLISTWIDGET_H
#define VISHNU_ZEQGROUPLISTWIDGET_H

#include <string>
#include <vector>

#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

#include <sp1common/sp1common.h>

#include "ZEQGroupWidget.h"

namespace vishnu
{

  class ZEQGroupListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit ZEQGroupListWidget( QWidget* parent = Q_NULLPTR );

      ZEQGroupWidgetPtr syncGroup( const std::string& key, const QString& name,
        const QString& owner, const std::vector< std::string >& ids,
        const QColor& color );

      void changeGroupName( const std::string& key, const QString& name );

      void changeGroupColor( const std::string& key, const QColor& color );

      void removeCurrentGroup( );

      void removeGroup( const std::string& key );

      std::string getKey( QListWidgetItem* listWidgetItem );

    private:

      ZEQGroupWidgetPtr findGroup( const std::string& key );

    signals:

      void addDataSetEvent( const std::string& filePath );

  };

}

#endif
