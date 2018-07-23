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

  typedef std::shared_ptr< DataSet > DataSetPtr;
  typedef std::map< std::string, DataSetPtr > DataSets;

  //typedef std::shared_ptr< DataSetWidget > DataSetWidgetPtr;
  typedef DataSetWidget* DataSetWidgetPtr;
  typedef std::vector< DataSetWidgetPtr > DataSetWidgets;

  class DataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit DataSetListWidget( QWidget* parent = Q_NULLPTR );

      DataSetWidgets addDataSets( const std::vector< std::string >& dropped =
        std::vector< std::string >( ) );

      std::vector< std::string > getPropertiesToRemove( );

      void removeCurrentDataSet( );

      DataSets getDataSets( );

      std::vector< std::string > getCommonProperties( );

    protected:

      void dragEnterEvent( QDragEnterEvent* event );

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

      void dropEvent( QDropEvent* event );

    signals:

      void addDataSetsEvent( const std::vector< std::string >& files );

    private:

      void createDataSetFromCSV( const std::string& path,
        DataSetWidgets& dataSetWidgets );

  };

}

#endif
