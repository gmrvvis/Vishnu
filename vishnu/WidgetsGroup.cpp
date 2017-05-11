#include "WidgetsGroup.hpp"

namespace vishnu
{
  WidgetsGroup::WidgetsGroup( QLabel* name, QLabel* owner, QPushButton* button )
  {
    _name = name;
    _owner = owner;
    _button = button;
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
