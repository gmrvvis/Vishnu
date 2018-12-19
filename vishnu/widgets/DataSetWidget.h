/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_DATASETWIDGET_H
#define VISHNU_DATASETWIDGET_H

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <string>
#include <vector>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class DataSetWidget;
  using DataSetWidgetPtr = DataSetWidget*;
  using DataSetWidgets = std::vector< DataSetWidgetPtr >;

  class DataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit DataSetWidget( const sp1common::DataSetPtr& dataSet,
      QWidget* parent = Q_NULLPTR );

    std::string getPath( void ) const;
    void setPath( const std::string& path );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

    sp1common::DataSetPtr getDataSet( void ) const;

  signals:

    void removeSelected( void );
    void updatePath( void );

  private slots:

    void clickPath( void );
    void clickRemove( void );

  private:

    QLabel* _path = nullptr;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;
    sp1common::DataSetPtr _dataSet;

  };

}

#endif
