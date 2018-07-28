#ifndef VISHNU_DATASETWIDGET_H
#define VISHNU_DATASETWIDGET_H

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <string>
#include <vector>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class DataSetWidget;
  typedef DataSetWidget* DataSetWidgetPtr;
  typedef std::vector< DataSetWidgetPtr > DataSetWidgets;

  class DataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit DataSetWidget( const std::string& path,
      QWidget* parent = Q_NULLPTR );

    std::string getPath( void ) const;
    void setPath( const std::string& path );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

    std::vector< std::string > getHeaders( ) const;

  signals:

    void removeSelected( void );
    void updatePath( void );

  private slots:

    void clickPath( void );
    void clickRemove( void );

  private:

    QLabel* _path = nullptr;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;
    std::vector< std::string > _headers;

  };

}

#endif
