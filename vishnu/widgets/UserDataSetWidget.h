#ifndef VISHNU_USERDATASETWIDGET_H
#define VISHNU_USERDATASETWIDGET_H

#include <QString>
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

    explicit UserDataSetWidget( const QString& name, const QString& path,
      const QString& csvFilename, const QString& xmlFilename,
      const bool& selected, QWidget* parent = Q_NULLPTR );

    QString getName( void ) const;
    QString getPath( void ) const;
    QString getCsvFilename( void ) const;
    QString getCsvPath( void ) const;
    QString getXmlFilename( void ) const;
    QString getXmlPath( void ) const;

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
    QString _path;
    QString _csvFilename;
    QLabel* _csvPath = nullptr;
    QString _xmlFilename;
    QLabel* _xmlPath = nullptr;
    QCheckBox _selected;
    QPushButton* _remove = nullptr;
    QListWidgetItem* _listWidgetItem = nullptr;

    void setName( const QString& name );
    void setCsvPath( const QString& path, const QString& csvFilename );
    void setXmlPath( const QString& path, const QString& xmlFilename );

  };

}

#endif
