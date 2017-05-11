#include "SyncGroup.hpp"

namespace vishnu
{

  SyncGroup::SyncGroup( )
  {

  }

  SyncGroup::SyncGroup( const std::string& key, const std::string& name, const std::string& owner,
    const std::vector<std::string>& ids, const unsigned int& red, const unsigned int& green, const unsigned int& blue )
  {
    _key = key;
    _name = name;
    _owner = owner;
    _ids = ids;
    _color = QColor(red, green, blue);
  }

  SyncGroup::~SyncGroup( )
  {

  }

  std::string SyncGroup::getKey( )
  {
      return _key;
  }

  std::string SyncGroup::getName( )
  {
      return _name;
  }

  std::string SyncGroup::getOwner( )
  {
      return _owner;
  }

  std::vector<std::string> SyncGroup::getIds( )
  {
      return _ids;
  }

  QColor SyncGroup::getColor( )
  {
      return _color;
  }

  void SyncGroup::setKey( const std::string& key )
  {
      _key = key;
  }

  void SyncGroup::setName( const std::string& name )
  {
      _name = name;
  }

  void SyncGroup::setOwner( const std::string& owner )
  {
      _owner = owner;
  }

  void SyncGroup::setIds( const std::vector<std::string>& ids)
  {
      _ids = ids;
  }

  void SyncGroup::setColor( const QColor& color )
  {
      _color = color;
  }


}
