#ifndef VISHNU_USERDATASETWIDGET_H
#define VISHNU_USERDATASETWIDGET_H

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include <string>
#include <vector>
#include <memory>

#include <sp1common/sp1common.h>

namespace vishnu
{

  class UserDataSetWidget;
  typedef UserDataSetWidget* UserDataSetWidgetPtr;
  typedef std::vector< UserDataSetWidgetPtr > UserDataSetWidgets;

  class UserDataSetWidget : public QWidget
  {

    Q_OBJECT

  public:

    explicit UserDataSetWidget( const std::string& name,
      const std::string& path, const std::string& csvFilename,
      const std::string& xmlFilename, const bool& selected,
      QWidget* parent = Q_NULLPTR );

    std::string getName( void ) const;
    std::string getPath( void ) const;
    std::string getCsvFilename( void ) const;
    std::string getCsvPath( void ) const;
    std::string getXmlFilename( void ) const;
    std::string getXmlPath( void ) const;

    bool getSelected( void ) const;
    void setSelected( const bool& selected );

    QListWidgetItem* getListWidgetItem( void ) const;
    void setListWidgetItem( QListWidgetItem* listWidgetItem );

  signals:

    void signalCheckSelected( int checked );
    void signalRemoveSelected( void );
    void updateName( void );
    void updateCheckBox( void );

  private slots:

    void clickName( void );
    void slotCheck( int checked );
    void slotRemove( void );

  private:

    sp1common::ClickableLabel* _name = nullptr;
    std::string _path;
    std::string _csvFilename;
    QLabel* _csvPath = nullptr;
    std::string _xmlFilename;
    QLabel* _xmlPath = nullptr;
    QCheckBox* _selected = nullptr;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;

    void setName( const std::string& name );
    void setCsvPath( const std::string& path, const std::string& csvFilename );
    void setXmlPath( const std::string& path, const std::string& xmlFilename );

  };

}

#endif
