#ifndef VISHNU_USERDATASETLISTWIDGET_H
#define VISHNU_USERDATASETLISTWIDGET_H

#include <string>
#include <vector>
#include <map>
#include <memory>

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
  typedef std::map< std::string, UserDataSetPtr > UserDataSetMap;

  typedef std::shared_ptr< UserDataSetWidget > UserDataSetWidgetPtr;
  typedef std::vector< UserDataSetWidgetPtr > UserDataSetWidgets;

  class UserDataSetListWidget : public QListWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetListWidget( QWidget* parent = Q_NULLPTR );

    UserDataSetWidgetPtr addUserDataSet( const std::string& name,
      const std::string& path, const std::string& csvFilename,
      const std::string& xmlFilename, const bool& selected );

    UserDataSetWidgetPtr addUserDataSet( const UserDataSetPtr& userDataSet );

    void removeCurrentDataSet( );

    UserDataSetMap getDataSets( );

    protected:

    void dragMoveEvent( QDragMoveEvent* event );
    void dragLeaveEvent( QDragLeaveEvent* event );

  signals:

    void addUserDataSetEvent( const std::string& filePath );

  };

}

#endif
