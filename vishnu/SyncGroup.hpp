#ifndef SYNCGROUP_H
#define SYNCGROUP_H

#include <QColor>
#include <string>
#include <vector>

namespace vishnu
{
  class SyncGroup
  {
  public:
    SyncGroup( );
    SyncGroup( const std::string& key, const std::string& name,
      const std::string& owner, const std::vector<std::string>& ids,
      const unsigned int& red, const unsigned int& green,
      const unsigned int& blue );
    ~SyncGroup();

    std::string getKey( );
    std::string getName( );
    std::string getOwner( );
    std::vector<std::string> getIds( );
    QColor getColor( );

    void setKey( const std::string& key );
    void setName( const std::string& name );
    void setOwner( const std::string& owner );
    void setIds( const std::vector<std::string>& ids );
    void setColor( const QColor& color );

  private:
    std::string _key;
    std::string _name;
    std::string _owner;
    std::vector<std::string> _ids;
    QColor _color;
  };
}

#endif
