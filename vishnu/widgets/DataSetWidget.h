#ifndef VISHNU_DATASETWIDGET_H
#define VISHNU_DATASETWIDGET_H

#include <string>
#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <sp1common/sp1common.h>

namespace vishnu
{

  class DataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit DataSetWidget( const std::string& name,
      const std::string& path, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    void setName( const std::string& name );

    std::string getPath( void ) const;
    void setPath( const std::string& path );

    bool getChecked( void ) const;

    void setChecked( const bool& checked );

    QListWidgetItem* getListWidgetItem( void ) const;

    void setListWidgetItem( QListWidgetItem* listWidgetItem );

    std::vector< std::string > getHeaders( );

  signals:

    void removeSelected( void );
    void updateName( void );
    void updatePath( void );
    void updateCheckBox( void );

  private slots:

    void clickName( void );
    void clickPath( void );
    void clickCheckBox( void );
    void clickRemove( void );

  private:

    sp1common::ClickableLabel* _name = nullptr;
    QLabel* _path = nullptr;
    QCheckBox _checkbox;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;
    std::vector< std::string > _headers;

  };

}

#endif
