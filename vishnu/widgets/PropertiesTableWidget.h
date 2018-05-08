#ifndef VISHNU_PROPERTIESTABLEWIDGET_H
#define VISHNU_PROPERTIESTABLEWIDGET_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <QListWidgetItem>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>

#include <sp1common/sp1common.h>
#include "../Property.h"

namespace vishnu
{

  typedef std::shared_ptr< Property > PropertyPtr;
  typedef std::vector< PropertyPtr > Properties;

  class PropertiesTableWidget : public QTableWidget
  {

    Q_OBJECT

  public:

    explicit PropertiesTableWidget( QWidget* parent = Q_NULLPTR );

    void addProperties( const std::vector< std::string >& properties );

    void removeProperties( const std::vector< std::string >& properties );

    Properties getProperties( );

  };

}

#endif
