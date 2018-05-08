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

  class DataSetListWidget : public QListWidget
  {

    Q_OBJECT

  public:

    explicit DataSetListWidget( QWidget* parent = Q_NULLPTR );

    DataSetWidget* addDataSet( const std::string& dropped = std::string( ) );

    std::vector< std::string > getPropertiesToRemove( );

    void removeCurrentDataSet( );

    DataSets getDataSets( );

    protected:

    void dragEnterEvent( QDragEnterEvent* event );
    void dragMoveEvent( QDragMoveEvent* event );
    void dragLeaveEvent( QDragLeaveEvent* event );
    void dropEvent( QDropEvent* event );

  signals:

    void addDataSetEvent( const std::string& filePath );

  };

}

#endif
