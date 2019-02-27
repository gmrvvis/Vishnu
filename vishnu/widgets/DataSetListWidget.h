/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

// vishnucommon.
#include <vishnucommon/vishnucommon.h>

#ifdef USE_ESPINA

// VTK
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

// EspINA.
#include <EspinaCore/Analysis/Analysis.h>
#include <EspinaCore/Analysis/Extensions.h>
#include <EspinaCore/Analysis/ItemExtension.hxx>
#include <EspinaCore/Analysis/Segmentation.h>
#include <EspinaCore/Analysis/Channel.h>
#include <EspinaCore/Analysis/Data/MeshData.h>
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
  using DataSetListWidgetPtr = std::shared_ptr< DataSetListWidget >;

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

      vishnucommon::DataSetsPtr getDataSets( void ) const;

      vishnucommon::PropertyGroupsPtr getPropertyGroups( void ) const;

      void setPropertyGroups(
        const vishnucommon::PropertyGroupsPtr& propertyGroups );

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

      // EspINA segmentations -> Meshes (OBJ).
      void generateSegmentationMeshes( const std::string& segmentationMeshesRoot_, ESPINA::SegmentationList segmentations_ );

      // vtkPolyData -> OBJ.
      std::string vtkPolyDataToOBJ( vtkSmartPointer< vtkPolyData > polyData );

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
      vishnucommon::Properties segsJsonSchemaToSP1Properties( QString jsonSchema );

      /** END EspINA methods. **/
#endif
      void createDataSetsFromJSON( DataSetWidgets& dataSetWidgets,
        const std::string& path );

      void createDataSetFromCSV( DataSetWidgets& dataSetWidgets,
        const std::string& path,
        const vishnucommon::Properties& properties = vishnucommon::Properties( ) );

      vishnucommon::PropertyGroupsPtr _propertyGroups;

  };

}

#endif
