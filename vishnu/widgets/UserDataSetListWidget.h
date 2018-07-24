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
#include "../UserDataSet.h"

namespace vishnu
{

  typedef std::shared_ptr< UserDataSet > UserDataSetPtr;
  typedef std::map< std::string, UserDataSetPtr > UserDataSets;

  typedef std::shared_ptr< UserDataSetWidget > UserDataSetWidgetPtr;
  typedef std::vector< UserDataSetWidgetPtr > UserDataSetWidgets;

  class UserDataSetListWidget : public QListWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetListWidget( QWidget* parent = Q_NULLPTR );

    UserDataSetWidgets addUserDataSet( const std::string& name,
      const std::string& csvPath, const std::string& xmlPath,
      const bool& selected );

    void removeCurrentDataSet( );

    UserDataSets getDataSets( );

    protected:

    void dragMoveEvent( QDragMoveEvent* event );
    void dragLeaveEvent( QDragLeaveEvent* event );

  signals:

    void addUserDataSetEvent( const std::string& filePath );

  };

}

#endif
