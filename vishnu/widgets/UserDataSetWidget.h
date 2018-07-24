#ifndef VISHNU_USERDATASETWIDGET_H
#define VISHNU_USERDATASETWIDGET_H

#include <string>
#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetWidget( const std::string& name,
      const std::string& csvPath, const std::string& xmlPath,
      const bool& selected, QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    void setName( const std::string& name );

    std::string getCsvPath( void ) const;
    void setCsvPath( const std::string& csvPath );

    std::string getXmlPath( void ) const;
    void setXmlPath( const std::string& xmlPath );

    bool getSelected( void ) const;
    void setSelected( const bool& selected );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

  signals:

    void removeSelected( void );
    void updateName( void );
    void updateCheckBox( void );

  private slots:

    void clickName( void );
    void clickCheckBox( void );
    void clickRemove( void );

  private:

    sp1common::ClickableLabel* _name = nullptr;
    QLabel* _csvPath = nullptr;
    QLabel* _xmlPath = nullptr;
    QCheckBox _selected;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;

  };

}

#endif
