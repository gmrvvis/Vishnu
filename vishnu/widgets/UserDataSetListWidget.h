#ifndef VISHNU_USERDATASETLISTWIDGET_H
#define VISHNU_USERDATASETLISTWIDGET_H

#include <string>
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

  class UserDataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit UserDataSetListWidget( QWidget* parent = Q_NULLPTR );

      UserDataSetWidgetPtr addUserDataSet( const std::string& name,
        const std::string& path, const std::string& csvFilename,
        const std::string& jsonFilename, const std::string& xmlFilename,
        const bool& selected );

      void removeCurrentDataSet( );

      UserDataSetMap getDataSets( );

      void clearDataSets( void );

    protected:

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

    signals:

      void addUserDataSetEvent( const std::string& filePath );

    private slots:

      void slotCheckSelectedDataSets( bool checked );

    private:

      bool _checkingProperty;

  };

}

#endif
