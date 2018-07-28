#ifndef VISHNU_DATASETWINDOW_H
#define VISHNU_DATASETWINDOW_H

#include <QDialog>
#include <QAction>
#include <QToolBar>
#include <QPushButton>
#include <QDir>

#include <string>
#include <vector>

#include "widgets/PathsWidget.h"
#include "widgets/DataSetListWidget.h"
#include "widgets/PropertiesTableWidget.h"

Q_DECLARE_METATYPE( std::vector< std::string > )

namespace vishnu
{

  class DataSetWindow : public QDialog
  {
      Q_OBJECT

      public:
        explicit DataSetWindow( QWidget *parent = Q_NULLPTR );
        ~DataSetWindow();

      private slots:
        void slotCreateButton( void );
        void slotCancelButton( void );
        void slotAddFiles( const std::vector< std::string >& dropped =
          std::vector< std::string >( ) );
        void slotRemoveDataSet( );

      private:
        QToolBar* _toolBar;
        PathsWidgetPtr _pathsWidget;
        DataSetListWidgetPtr _dataSetListWidget;
        PropertiesTableWidgetPtr _propertiesTableWidget;
        QPushButton* _cancelButton;
        QPushButton* _createButton;

        bool createCSV( const std::string& csvPath,
          const sp1common::PropertyGroupsPtr& propertyGroups );
        bool createXML( const std::string& path, const std::string& csvPath,
          const std::string& xmlPath,
          const sp1common::DataSetPtr& resultDataSet,
          const sp1common::PropertyGroupsPtr& propertyGroups );
        bool generateDataFiles( QDir dir );

  };

}

#endif
