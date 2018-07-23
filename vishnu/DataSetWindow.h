#ifndef VISHNU_DATASETWINDOW_H
#define VISHNU_DATASETWINDOW_H

#include <QDialog>
#include <QAction>
#include <QToolBar>
#include <QPushButton>
#include <QDir>

#include <string>
#include <vector>

#include "widgets/DataSetListWidget.h"
#include "widgets/PropertiesTableWidget.h"

Q_DECLARE_METATYPE( std::vector< std::string > );

namespace vishnu
{
  typedef std::shared_ptr< DataSetListWidget > DataSetListWidgetPtr;
  typedef std::shared_ptr< PropertiesTableWidget > PropertiesTableWidgetPtr;

  class DataSetWindow : public QDialog
  {
      Q_OBJECT

      public:
        explicit DataSetWindow( QWidget *parent = Q_NULLPTR );
        ~DataSetWindow();

      private slots:
        void slotCreateButton( void );
        void slotCancelButton( void );
        void addDataSetItems( const std::vector< std::string >& dropped =
          std::vector< std::string>( ) );
        void removeDataSetItem( );

      private:
        QToolBar* _toolBar;
        DataSetListWidgetPtr _dataSetListWidget;
        PropertiesTableWidgetPtr _propertiesTableWidget;
        QPushButton* _cancelButton;
        QPushButton* _createButton;

        //QAction* _addDataSetsAction;
        //QAction* _removeDataSetAction;

        bool generateDataFiles( QDir dir );


  };

}

#endif
