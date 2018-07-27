#ifndef VISHNU_USERDATASETLISTWIDGET_H
#define VISHNU_USERDATASETLISTWIDGET_H

#include <vector>
#include <map>
#include <memory>

#include <QString>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

#include <sp1common/sp1common.h>

#include "UserDataSetWidget.h"
#include "../UserDataSets.h"

namespace vishnu
{
  typedef std::map< QString, UserDataSetPtr > UserDataSetMap;

  typedef std::shared_ptr< UserDataSetWidget > UserDataSetWidgetPtr;
  typedef std::vector< UserDataSetWidgetPtr > UserDataSetWidgets;

  class UserDataSetListWidget : public QListWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetListWidget( QWidget* parent = Q_NULLPTR );

    UserDataSetWidgetPtr addUserDataSet( const QString& name,
      const QString& path, const QString& csvFilename,
      const QString& xmlFilename, const bool& selected );

    UserDataSetWidgetPtr addUserDataSet( const UserDataSetPtr& userDataSet );

    void removeCurrentDataSet( );

    UserDataSetMap getDataSets( );

    protected:

    void dragMoveEvent( QDragMoveEvent* event );
    void dragLeaveEvent( QDragLeaveEvent* event );

  signals:

    void addUserDataSetEvent( const QString& filePath );

  };

}

#endif
