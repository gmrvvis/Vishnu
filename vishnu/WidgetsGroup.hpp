#ifndef WIDGETSGROUP_H
#define WIDGETSGROUP_H

#include <QLabel>
#include <QPushButton>

namespace vishnu
{
    class WidgetsGroup
    {
    public:
        WidgetsGroup( QLabel* name, QLabel* owner, QPushButton* button );
        ~WidgetsGroup( );

        QLabel* getNameLabel( ) const;
        QLabel* getOwnerLabel( ) const;
        QPushButton* getClosePushButton( ) const;

    private:
        QLabel* _name = nullptr;
        QLabel* _owner = nullptr;
        QPushButton* _button = nullptr;
    };
}

#endif
