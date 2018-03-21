#ifndef VISHNU_DATASETWIDGET_HPP
#define VISHNU_DATASETWIDGET_HPP

#include <string>
#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>

namespace vishnu
{

  class DataSetWidget : public QWidget
  {
    Q_OBJECT

  public:

    explicit DataSetWidget( const std::string& name,
      const std::string& path, QWidget* parent = Q_NULLPTR );

    std::string getName( ) const;
    void setName( const std::string& name );

    std::string getPath( ) const;
    void setPath( const std::string& path );

    bool getChecked( ) const;
    void setChecked( const bool& checked );

  private:

    QLabel* _name = nullptr;
    QLabel* _path = nullptr;
    QCheckBox _checkbox;

  };

}

#endif
