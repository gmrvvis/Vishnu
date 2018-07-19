#ifndef VISHNU_DATASETWINDOW_H
#define VISHNU_DATASETWINDOW_H

#include <QWindow>

namespace vishnu
{

  class DataSetWindow : public QWindow
  {
      Q_OBJECT

      public:
        explicit DataSetWindow( QWidget *parent = 0 );
        ~DataSetWindow();

  };

}

#endif
