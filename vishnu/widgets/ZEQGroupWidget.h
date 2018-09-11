/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_ZEQGROUPWIDGET_H
#define VISHNU_ZEQGROUPWIDGET_H

#include <string>
#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <sp1common/sp1common.h>

namespace vishnu
{  
  class ZEQGroupWidget;
  typedef ZEQGroupWidget* ZEQGroupWidgetPtr;

  class ZEQGroupWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit ZEQGroupWidget( const std::string& key, const QString& name,
      const QString& owner, const std::vector<std::string>& ids,
      const QColor& color, QWidget* parent = Q_NULLPTR );

    std::string getKey( void ) const;

    QString getName( void ) const;
    void setName( const QString& name );

    QString getOwner( void ) const;
    void setOwner( const QString& owner );

    QColor getColor( void ) const;
    void setColor( const QColor& color );

    std::vector< std::string > getIds( void ) const;
    void setIds( const std::vector< std::string >& ids );

    QListWidgetItem* getListWidgetItem( void ) const;

    void setListWidgetItem( QListWidgetItem* listWidgetItem );

  signals:

    void signalRemoveSelectedGroup( );
    void signalRemoveGroup( const QString& );
    void signalChangeGroupName( const QString&, const QString& );
    void signalChangeGroupColor( const QString&, const QColor& );

  private slots:

    void clickRemove( void );

  private:

    std::string _key;
    QLabel* _name = nullptr;
    QLabel* _owner = nullptr;
    QLabel* _color = nullptr;
    std::vector< std::string > _ids;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;
  };

}

#endif