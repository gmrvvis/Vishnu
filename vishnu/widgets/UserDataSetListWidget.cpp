#include "UserDataSetListWidget.h"

#include <algorithm>

#include <QListWidgetItem>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>

#include "../Definitions.hpp"
#include "../RegExpInputDialog.h"

namespace vishnu
{
  UserDataSetListWidget::UserDataSetListWidget( QWidget* /*parent*/ )
  {    
    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::DragDrop );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );
  }

  UserDataSetWidgetPtr UserDataSetListWidget::addUserDataSet(
    const std::string& name, const std::string& path,
    const std::string& csvFilename, const std::string& xmlFilename,
    const bool& selected )
  {
    //UserDataSetWidgets dataSetWidgets;
/*
    //Format basename to 10 chars without spaces
    std::string name;
    std::string tempName = std::string( );
    bool validName = false;
    bool notUsedName = true;

    do
    {
      //Check if it's a valid name
      QRegularExpression regularExpression("[A-Za-z0-9]{1,10}$");
      std::string dataSetName = RegExpInputDialog::getText(this, "DataSet name",
        "Enter DataSet name: " + filePath, std::string::fromStdString( tempName ),
        regularExpression, &validName);
      if ( validName )
      {
        name = dataSetName.toStdString( );
        //Check if name doesn't exist
        notUsedName = true;
        for( int j = 0; j < count( ); ++j )
        {
          UserDataSetWidget* widget = static_cast< UserDataSetWidget* >(
            itemWidget( item( j ) ) );
          if ( widget->getName( ) == name )
          {
            QMessageBox::warning( this, APPLICATION_NAME,
              tr("Invalid name. This name already exists.") );
            notUsedName = false;
            break;
          }
        }
      }
    } while( !validName || !notUsedName );
*/
    //Add to dataset
    UserDataSetWidgetPtr dataSetWidget( new UserDataSetWidget( name, path,
      csvFilename, xmlFilename, selected ) );
    dataSetWidget->setListWidgetItem( new QListWidgetItem( this ) );

    QListWidgetItem* listWidgetItem = dataSetWidget->getListWidgetItem( );
    addItem( listWidgetItem );
    listWidgetItem->setSizeHint( dataSetWidget->sizeHint ( ) );
    setItemWidget( listWidgetItem, dataSetWidget.get( ) );

    //dataSetWidgets.emplace_back( dataSetWidget );

    return dataSetWidget;
  }

  UserDataSetWidgetPtr UserDataSetListWidget::addUserDataSet(
    const UserDataSetPtr& userDataSet )
  {
    return addUserDataSet( userDataSet->getName( ), userDataSet->getPath( ),
      userDataSet->getCsvFilename( ), userDataSet->getXmlFilename( ),
      userDataSet->getSelected( ) );
  }

  void UserDataSetListWidget::removeCurrentDataSet( )
  {    
    takeItem( row( currentItem( ) ) );
  }

  UserDataSetMap UserDataSetListWidget::getDataSets( )
  {
    UserDataSetMap dataSets;

    for( int row = 0; row < count( ); ++row )
    {
      UserDataSetWidget* dsw = static_cast< UserDataSetWidget* >(
        itemWidget( item( row ) ) );

      UserDataSetPtr dataSet( new UserDataSet( dsw->getName( ), dsw->getPath( ),
        dsw->getCsvFilename( ), dsw->getXmlFilename( ), dsw->getSelected( ) ) );
      dataSets[ dsw->getName( ) ] = dataSet;

    }

    return dataSets;
  }

  void UserDataSetListWidget::dragMoveEvent( QDragMoveEvent* event )
  {
    event->acceptProposedAction();
  }

  void UserDataSetListWidget::dragLeaveEvent( QDragLeaveEvent* event )
  {
    event->accept();
  }
}
