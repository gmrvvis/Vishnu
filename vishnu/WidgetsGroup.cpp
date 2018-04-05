#include "WidgetsGroup.h"

namespace vishnu
{
  WidgetsGroup::WidgetsGroup( QLabel* name, QLabel* owner, QPushButton* button )
  {
    _name = name;
    _owner = owner;
    _button = button;

    _button->setStyleSheet(
      "QPushButton:hover:!pressed { background-color: rgb(167, 205, 255);"
      "border-style: outset;"
      "border-width: 1px; border-color: green; }"

      "QPushButton:hover:pressed { background-color: rgb(167, 205, 255);"
      "border-style: outset;"
      "border-width: 1px; border-color: red; }"

      "QPushButton {background-image: url(://image/settings.png);background-color: rgb(167, 205, 255);"
      "border-style: outset;"
      "border-width: 1px; border-color: beige; "
      "padding-left: 10px;"
      "text-align: right;"
      "padding-right: 20px;"
      "background-position: center left;"
      "background-repeat: no-repeat;"
      "background-origin: content;    }"

    );
  }
  
  WidgetsGroup::~WidgetsGroup( )
  {
    _name = nullptr;
    _owner = nullptr;
    _button = nullptr;
  }

  QLabel* WidgetsGroup::getNameLabel( ) const
  {
    return _name;
  }

  QLabel* WidgetsGroup::getOwnerLabel( ) const
  {
    return _owner;
  }

  QPushButton* WidgetsGroup::getClosePushButton( ) const
  {
    return _button;
  }

}
