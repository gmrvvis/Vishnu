#ifndef VISHNU_DATASETLISTWIDGET_H
#define VISHNU_DATASETLISTWIDGET_H

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

#include "DataSetWidget.h"
#include "../DataSet.h"

namespace vishnu
{

  class DataSetListWidget;
  typedef std::shared_ptr< DataSetListWidget > DataSetListWidgetPtr;

  class DataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit DataSetListWidget( QWidget* parent = Q_NULLPTR );

      DataSetWidgets addDataSets( const std::vector< std::string >& dropped =
        std::vector< std::string >( ) );

      std::vector< std::string > getPropertiesToRemove( );

      void removeCurrentDataSet( );

      std::vector< std::string > getCommonProperties( );

      DataSets getDataSets( );

    protected:

      void dragEnterEvent( QDragEnterEvent* event );

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

      void dropEvent( QDropEvent* event );

    signals:

      void signalAddFiles( const std::vector< std::string >& files );

    private:

      void createDataSetFromCSV( const std::string& path,
        DataSetWidgets& dataSetWidgets );

  };

}

#endif
