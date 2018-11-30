/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_DATASETLISTWIDGET_H
#define VISHNU_DATASETLISTWIDGET_H

// std.
#include <string>
#include <vector>
#include <map>
#include <memory>

// Qt.
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

// sp1common.
#include <sp1common/sp1common.h>

#ifdef USE_ESPINA
// EspINA.
#include <EspinaCore/Analysis/Analysis.h>
#include <EspinaCore/Analysis/Extensions.h>
#include <EspinaCore/Analysis/ItemExtension.hxx>
#include <EspinaCore/Analysis/Segmentation.h>
#include <EspinaCore/Analysis/Channel.h>
#include <EspinaCore/Analysis/Data/VolumetricData.hxx>
#include <EspinaCore/Analysis/Filters/VolumetricStreamReader.h>
#include <EspinaCore/Analysis/Sample.h>
#include <EspinaCore/Analysis/Query.h>
#include <EspinaCore/Factory/AnalysisReader.h>
#include <EspinaCore/Factory/CoreFactory.h>
#include <EspinaCore/Factory/FilterFactory.h>
#include <EspinaCore/IO/DataFactory/RawDataFactory.h>
#include <EspinaCore/IO/SegFile.h>
#include <EspinaCore/IO/ErrorHandler.h>
#include <EspinaCore/MultiTasking/Scheduler.h>
#include <EspinaCore/Readers/ChannelReader.h>
#include <EspinaCore/Utils/ListUtils.hxx>
#include <EspinaCore/Plugin.h>
#include <EspinaCore/Types.h>
#include <EspinaExtensions/LibraryExtensionFactory.h>
#endif

// Qt.
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QUuid>

#include "DataSetWidget.h"
#ifdef USE_ESPINA
#include "../espinaExtensions/ExtensionInformationDump.h"
#endif

namespace vishnu
{

  class DataSetListWidget;
  typedef std::shared_ptr< DataSetListWidget > DataSetListWidgetPtr;

  class DataSetListWidget : public QListWidget
  {

    Q_OBJECT

    public:

      explicit DataSetListWidget( QWidget* parent = Q_NULLPTR );

      DataSetWidgets addDataSets( const std::vector< std::string >& dropped =
        std::vector< std::string >( ) );

      std::vector< std::string > getPropertiesToRemove( void );

      void removeCurrentDataSet( void );

      std::vector< std::string > getCommonProperties( void );

      sp1common::DataSetsPtr getDataSets( void ) const;

      sp1common::PropertyGroupsPtr getPropertyGroups( void ) const;

      void setPropertyGroups(
        const sp1common::PropertyGroupsPtr& propertyGroups );

    protected:

      void dragEnterEvent( QDragEnterEvent* event );

      void dragMoveEvent( QDragMoveEvent* event );

      void dragLeaveEvent( QDragLeaveEvent* event );

      void dropEvent( QDropEvent* event );

    signals:

      void signalAddFiles( const std::vector< std::string >& files );

    private:

#ifdef USE_ESPINA
      /** BEGIN EspINA methods. **/

      void createDataSetsFromSEG( DataSetWidgets& dataSetWidgets,
        const std::string& path );

      // EspINA segmentations -> Segmentations CSV.
      QString getCSVFromSegmentations( ESPINA::AnalysisPtr analysis,
                                       ESPINA::SegmentationList segmentations );

      // Based on ESPINA::GUI::availableInformation.
      QMap< QString, QStringList > segmentationsAvailableInformation( ESPINA::SegmentationList segmentations );

      // Segmentations CSV -> Segmentations JSON schema.
      // Manual parsing (in the future we could do it (semi-)automatically).
      QString createJsonSchema( QString csvString );
      QJsonObject createAttributeObject( QString attributeName );
      QJsonObject createAttributeObjectDFL( QString actualAttributeName );
      QJsonObject createAttributeObjectSEG( QString actualAttributeName );
      QJsonObject createAttributeObjectISS( QString actualAttributeName );
      QJsonObject createAttributeObjectNTS( QString actualAttributeName );
      QJsonObject createAttributeObjectTGS( QString actualAttributeName );
      QJsonObject createAttributeObjectEGD( QString actualAttributeName );
      QJsonObject createAttributeObjectSKL( QString actualAttributeName );
      QJsonObject createAttributeObjectMPH( QString actualAttributeName );
      QJsonObject createAttributeObjectSAS( QString actualAttributeName );
      QJsonObject createAttributeObjectSTI( QString actualAttributeName );
      QJsonObject createAttributeObjectSYN( QString actualAttributeName );
      QJsonObject createAttributeObjectAXN( QString actualAttributeName );
      QJsonObject createAttributeObjectDEN( QString actualAttributeName );

      // Segmentations JSON schema -> SP1 Properties.
      sp1common::Properties segsJsonSchemaToSP1Properties( QString jsonSchema );

      /** END EspINA methods. **/
#endif
      void createDataSetsFromJSON( DataSetWidgets& dataSetWidgets,
        const std::string& path );

      void createDataSetFromCSV( DataSetWidgets& dataSetWidgets,
        const std::string& path,
        const sp1common::Properties& properties = sp1common::Properties( ) );

      sp1common::PropertyGroupsPtr _propertyGroups;

  };

}

#endif
