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

#include "DataSetListWidget.h"

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

  DataSetListWidget::DataSetListWidget( QWidget* parent )
      : QListWidget( parent )
  {

    setSelectionMode( QAbstractItemView::SingleSelection );
    setDragDropMode( QAbstractItemView::InternalMove );
    setDefaultDropAction( Qt::MoveAction );
    setAcceptDrops( true );

    setMouseTracking( true );
    setStyleSheet(
      "QListWidget::item:selected{border: 1px solid #6a6a6a;background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #AAAAFF, stop: 0.5 #FFFFFF, stop: 1 #AAAAFF );}"
      "QListWidget::item:!selected{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.0 #CCCCFF, stop: 0.5 #FFFFFF, stop: 1.0 #CCCCFF );}"
      "QListWidget::item:hover{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #BBBBFF, stop: 0.5 #FFFFFF, stop: 1 #BBBBFF );}"
    );

     _propertyGroups.reset( new vishnucommon::PropertyGroups( ) );
  }

// -----------------------------------------------------------------------------

#ifdef USE_ESPINA

  class VishnuErrorHandler
  : public ESPINA::IO::ErrorHandler
  {

    public:

      virtual ~VishnuErrorHandler()
      { };

      virtual void warning(const QString& msg)
      {
        qDebug( ) << msg;
      };

      virtual void error(const QString& msg)
      {
        qDebug( ) << msg;
      };

      virtual QFileInfo fileNotFound(const QFileInfo& file,
                                     QDir dir = QDir(),
                                     const ESPINA::Core::Utils::SupportedFormats &filters = ESPINA::Core::Utils::SupportedFormats().addAllFormat(),
                                     const QString &hint = QString())
      {
        QString key = file.absoluteFilePath();

        if (!m_files.contains(key))
        {
          QString locatedFilename;

          if(defaultDir().exists(file.fileName()))
          {
            locatedFilename = defaultDir().filePath(file.fileName());
          }
          else
          {
            QString title     = (hint.isEmpty())? QObject::tr("Select file for %1:").arg(file.fileName()) : hint;
            QDir    directory = (dir == QDir()) ? defaultDir() : dir;

            // Temporary.
            auto filtersAux = filters;

            //locatedFilename = DefaultDialogs::OpenFile(title, filters, directory.absolutePath());
          }

          //if (!locatedFilename.isEmpty())
          //{
          //  m_files[key] = QFileInfo(locatedFilename);
          //}
        }

        return m_files.value(key, QFileInfo());
      };

    private:

      QMap<QString, QFileInfo> m_files;

  };

  /** BEGIN EspINA methods. **/

  void DataSetListWidget::createDataSetsFromSEG(
    DataSetWidgets& dataSetWidgets, const std::string& path )
  {
    // EspINA Core factory object.
    auto vishnuScheduler = std::make_shared< ESPINA::Scheduler >( 16000 );
    auto factory = std::make_shared< ESPINA::CoreFactory >( vishnuScheduler );

    // EspINA channel reader registered.
    auto vishnuChannelReader = std::make_shared<ESPINA::ChannelReader>();
    factory->registerAnalysisReader( vishnuChannelReader );
    factory->registerFilterFactory( vishnuChannelReader );

    // Registering extensions in Core factory object.
    std::cout << "Registering extensions in Core factory object..." << std::endl;
    factory->registerExtensionFactory(
      std::make_shared<ESPINA::LibrarySegmentationExtensionFactory>( factory.get( ) ) );
    factory->registerExtensionFactory(
      std::make_shared<ESPINA::LibraryStackExtensionFactory>( factory.get( ) ) );
    std::cout << "Registration ended." << std::endl;

    // Loading EspINA plugins.
    QDir pluginDir( QString("/home/jguerrero/opt/003_ALTERNATIVE/espina/build/App/plugins") );
    auto pluginLoaders = ESPINA::Core::loadPlugins( pluginDir, factory.get( ) );

    QFileInfo file( QString::fromStdString( path ) );

    try
    {
      // Loading EspINA analysis.
      std::cout << "Loading EspINA analysis called: " << path << std::endl;

      auto vishnuErrorHandler = std::make_shared<VishnuErrorHandler>();
      vishnuErrorHandler->setDefaultDir( file.absoluteDir() );

      auto analysis = ESPINA::IO::SegFile::load( file, factory,
                                                 nullptr, vishnuErrorHandler );

      std::cout << "EspINA analysis ready." << std::endl;

      auto segmentationList =
        ESPINA::Core::Utils::toRawList< ESPINA::Segmentation >(
        analysis->segmentations( ) );

      QString segmentationCSV = getCSVFromSegmentations( analysis.get( ), segmentationList );

      // Unloading EspINA plugins.
      ESPINA::Core::unloadPlugins( pluginLoaders );

      // Writing CSV to a file.
      /**/
      std::string segmentationCSVPath("segmentations.csv");
      remove( segmentationCSVPath.c_str( ) );
      std::ofstream ofs;
      ofs.exceptions( std::ofstream::failbit | std::ofstream::badbit );
      try
      {
        ofs.open( segmentationCSVPath.c_str( ), std::ofstream::out | std::ofstream::app );
      }
      catch( std::system_error& e )
      {
        std::cerr << e.code( ).message( ) << std::endl;
        exit( EXIT_FAILURE );
      }
      ofs << segmentationCSV.toStdString( ).c_str( );
      ofs.close( );
      /**/

      std::string segmentationMeshesRoot("geometricData");
      generateSegmentationMeshes( segmentationMeshesRoot, segmentationList );

      QString segmentationJSONSchema = createJsonSchema( segmentationCSV );
      vishnucommon::Properties segmentationProperties =
        segsJsonSchemaToSP1Properties( segmentationJSONSchema );

      createDataSetFromCSV( dataSetWidgets, segmentationCSVPath, segmentationProperties );

      // Saving SEG file.
      // SEGMENTATION FAULT HERE !
      ESPINA::IO::SegFile::save( analysis.get( ), file,
                                 nullptr, vishnuErrorHandler );
    }
    catch(const ESPINA::Core::Utils::EspinaException &e)
    {
      // catch and inform of any error.
      qDebug() << QString("Unable to load file %1. Error: %2.").arg( file.fileName( ) ).arg( e.details( ) );
    }
  }

  QString DataSetListWidget::getCSVFromSegmentations( ESPINA::AnalysisPtr analysis,
                                                      ESPINA::SegmentationList segmentations )
  {
    QString result;

    if(!segmentations.isEmpty())
    {
      const QString separator{","};
      const QString concatenator{";"};
      const QString newline{'\n'};
      QMap< QString, QStringList > availableInfo =
        segmentationsAvailableInformation( segmentations );

      // Feedback.
      /**
      QMap<QString, QStringList>::iterator it;
      for (it = availableInfo.begin(); it != availableInfo.end(); ++it)
      {
        std::cout << it.key().toStdString() << ": ";
        foreach( QString val, it.value( ) )
        {
          std::cout << val.toStdString() << ", ";
        }
        std::cout << std::endl;
      }
      **/

      // Feedback.
      std::cout << "Writing CSV header."  << std::endl;

      // create header row with different names.
      result += "DFLName,DFLAlias,SEGCategory,SEGConnections,";

      auto extensions = availableInfo.keys( );
      for(auto extensionType: extensions)
      {
        if(extensionType == "SegmentationIssues") continue;

        result += dumpExtensionHeaderToCSV(extensionType, availableInfo.value(extensionType));
        if(extensionType != extensions.last()) result += separator;
      }

      result += newline;

      // Feedback.
      unsigned int segmentationCounter = 1;
      for(auto segmentation: segmentations)
      {
        // Feedback.
        /**/
        std::cout << "Writing segmentation to the CSV: "
                  << segmentation->name().toStdString( )
                  << ". (" << segmentationCounter << "/"
                           << segmentations.size( )
                           << ")" << std::endl;
        /**/

        QString segmentationName = segmentation->name().simplified();
        QString segmentationAlias = segmentation->alias().simplified();

        if( segmentationName.isEmpty() && segmentationAlias.isEmpty() )
        {
          result += QString( "UnknownName" ) + QString( segmentationCounter );
          result += separator;
          result += QString( "UnknownAlias" ) + QString( segmentationCounter );
          result += separator;
        }
        else if( segmentationName.isEmpty() && !segmentationAlias.isEmpty() )
        {
          result += segmentationAlias;
          result += separator;
          result += segmentationAlias;
          result += separator;
        }
        else if( !segmentationName.isEmpty() && segmentationAlias.isEmpty() )
        {
          result += segmentationName;
          result += separator;
          result += segmentationName;
          result += separator;
        }
        else
        {
          result += segmentationName;
          result += separator;
          result += segmentationAlias;
          result += separator;
        }

        result += segmentation->category()->classificationName().simplified();
        result += separator;

        auto segConnections = analysis->connections(analysis->smartPointer(segmentation));

        for(int i = 0; i < segConnections.size(); ++i)
        {
          result += segConnections.at(i).segmentation2->name().simplified();

          if(i != segConnections.size() - 1) result += concatenator;
        }
        result += separator;

        for(auto extensionType: extensions)
        {
          // Feedback.
          // std::cout << "Extension type: " << extensionType.toStdString() << std::endl;

          if(extensionType == "SegmentationIssues") continue;

          ESPINA::Core::SegmentationExtension::KeyList keyList =
            availableInfo.value(extensionType);

          if( keyList.isEmpty() ) continue;

          result += dumpExtensionInformationToCSV(segmentation, extensionType, keyList);
          if(extensionType != extensions.last()) result += separator;
        }

        result += newline;

        // Feedback.
        segmentationCounter++;

        // One exec.
        //break;
      }

      // Feedback.
      std::cout << "CSV written." << std::endl;
    }

    return result;
  }

  void DataSetListWidget::generateSegmentationMeshes( const std::string& segmentationMeshesRoot_, ESPINA::SegmentationList segmentations_ )
  {
    QDir geometricDataDir( QString::fromStdString( segmentationMeshesRoot_ ) );
    if( !geometricDataDir.exists( ) )
    {
      int mkdirReturn =
        mkdir( segmentationMeshesRoot_.c_str( ),
               S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
      if( mkdirReturn == -1 )
      {
        std::cerr << "Error creating " << segmentationMeshesRoot_ << std::endl;
        exit( -1 );
      }
    }

    std::string segMeshesRootExtended = segmentationMeshesRoot_ + std::string( "/" );

    for( auto segmentation: segmentations_ )
    {
      auto segmentationOutput = segmentation->output( );
      if( !ESPINA::hasMeshData( segmentationOutput ) )
      {
        std::cout << "Segmentation " << segmentation->name( ).toStdString( ) << " / "
                                     << segmentation->alias( ).toStdString( )
                                     << " has not a mesh." << std::endl;
        continue;
      }
      auto segmentationPolyData = ESPINA::readLockMesh( segmentationOutput )->mesh( );
      std::string segmentationMeshOBJ = vtkPolyDataToOBJ( segmentationPolyData );

      QString segmentationName = segmentation->name().simplified();
      QString segmentationAlias = segmentation->alias().simplified();
      if( segmentationName.isEmpty() && segmentationAlias.isEmpty() )
      {
        // Highly unlikely.
        segmentationName = QString( "UnknownName" );
        segmentationAlias = QString( "UnknownAlias" );
      }
      else if( segmentationName.isEmpty() && !segmentationAlias.isEmpty() )
      {
        segmentationName = segmentationAlias;
      }
      else if( !segmentationName.isEmpty() && segmentationAlias.isEmpty() )
      {
        segmentationAlias = segmentationName;
      }

      auto segNameUScore = segmentationName.replace(' ','_');
      auto segAliasUScore = segmentationAlias.replace(' ','_');
      std::string segmentationMeshPath = segMeshesRootExtended + segNameUScore.toStdString( ) + "-" + segAliasUScore.toStdString( ) + "-mesh.obj";
      // Writing OBJ to a file.
      /**/
      remove( segmentationMeshPath.c_str( ) );
      std::ofstream ofs;
      ofs.exceptions( std::ofstream::failbit | std::ofstream::badbit );
      try
      {
        ofs.open( segmentationMeshPath.c_str( ), std::ofstream::out | std::ofstream::app );
      }
      catch( std::system_error& e )
      {
        std::cerr << e.code( ).message( ) << std::endl;
        exit( EXIT_FAILURE );
      }
      ofs << segmentationMeshOBJ.c_str( );
      ofs.close( );
      /**/
      std::cout << "Segmentation " << segmentation->name( ).toStdString( ) << " / "
                                   << segmentation->alias( ).toStdString( )
                                   << " mesh generated." << std::endl;

      // One exec.
      //break;
    }
  }

  std::string DataSetListWidget::vtkPolyDataToOBJ( vtkSmartPointer< vtkPolyData > polyData )
  {
    std::string meshOBJ("");

    // VTK objects and more.
    vtkCellArray* cells;
    vtkIdType     npts = 0;
    vtkIdType*    indx = 0;
    vtkPoints*    points;
    double*       point;
    unsigned int  currentIndex, i;
    unsigned int  idStart = 1;
    std::string   whiteSpace(" ");
    std::string   lineJump("\n");

    // Writing vertex position to the OBJ.
    points = polyData->GetPoints( );
    for( i = 0; i < points->GetNumberOfPoints( ); i++ )
    {
      point = points->GetPoint( i );

      std::stringstream xStream;
      xStream << std::fixed << std::setprecision( 2 ) << point[0];
      std::string xString = xStream.str();

      std::stringstream yStream;
      yStream << std::fixed << std::setprecision( 2 ) << point[1];
      std::string yString = yStream.str();

      std::stringstream zStream;
      zStream << std::fixed << std::setprecision( 2 ) << point[2];
      std::string zString = zStream.str();

      meshOBJ = meshOBJ + std::string("v ") + xString + whiteSpace
                                            + yString + whiteSpace
                                            + zString + lineJump;
    }
    points->Delete( );

    // No normals nor tex coords considered now.

    // Writing facet indices to the OBJ.
    if( polyData->GetNumberOfPolys( ) > 0 )
    {
      cells = polyData->GetPolys( );
      for( cells->InitTraversal( );
           cells->GetNextCell( npts, indx ); )
      {
        meshOBJ += std::string("f ");
        for( i = 0; i < npts; i++ )
        {
          currentIndex = static_cast< unsigned int >( indx[i] ) + idStart;
          meshOBJ = meshOBJ + std::to_string( currentIndex ) + whiteSpace;
        }
        meshOBJ += lineJump;
      }
    }

    return meshOBJ;
  }

  QMap< QString, QStringList > DataSetListWidget::segmentationsAvailableInformation( ESPINA::SegmentationList segmentations )
  {
    QMap< QString, QStringList > info;

    // Working with segmentation read-only extensions.
    for (auto segmentation : segmentations)
    {
      auto extensions = segmentation->readOnlyExtensions();

      for(auto extension: extensions)
      {
        if(!info.keys().contains(extension->type()))
        {
          for (auto key : extension->availableInformation())
          {
            info[key.extension()] << key;
          }
        }
        else
        {
          // fix for extensions with variable keys like stereological inclusion (variable number of counting frames).
          for(auto key: extension->availableInformation())
          {
            if(!info[key.extension()].contains(key))
            {
              info[key.extension()] << key;
            }
          }
        }
      }
    }

    for (auto tag : info.keys())
    {
      info[tag].removeDuplicates();
    }

    return info;
  }

  QString DataSetListWidget::createJsonSchema( QString csvString )
  {
    QJsonObject jsonSchemaObject;

    // Section: index.
    jsonSchemaObject.insert( "index",
                             QJsonValue::fromVariant( "segmentations" ) );

    // Parsing the CSV string.
    QTextStream csvStream( &csvString, QIODevice::ReadOnly );
    QString csvFirstLine = csvStream.readLine( );
    QStringList attributeNames = csvFirstLine.split( "," );

    // Section: attributes.
    QJsonObject attributesObject;
    for( QString attributeName : attributeNames )
    {
      QJsonObject attributeObject =
        createAttributeObject( attributeName );
      if( attributeObject.empty( ) )
      {
        qDebug( ) << attributeName << " is not a valid attribute name.";
        continue;
      }
      attributesObject.insert( attributeName, attributeObject );
    }
    jsonSchemaObject.insert( "attributes", attributesObject );

    // QJsonObject to QString.
    QJsonDocument jsonSchemaDocument( jsonSchemaObject );
    // QJsonDocument::Compact for better performance on writing/reading.
    QString jsonSchemaString(
      jsonSchemaDocument.toJson( QJsonDocument::Indented ) );
    return jsonSchemaString;
  }

  QJsonObject DataSetListWidget::createAttributeObject( QString attributeName )
  {
    QJsonObject attributeObject;

    QString extensionPrefix = attributeName.left( 3 );
    QString actualAttributeName =
      attributeName.right( attributeName.size( ) - 3 );
    if( extensionPrefix == "DFL" )
    {
      attributeObject = createAttributeObjectDFL( actualAttributeName );
    }
    else if( extensionPrefix == "SEG" )
    {
      attributeObject = createAttributeObjectSEG( actualAttributeName );
    }
    else if( extensionPrefix == "ISS" )
    {
      attributeObject = createAttributeObjectISS( actualAttributeName );
    }
    else if( extensionPrefix == "NTS" )
    {
      attributeObject = createAttributeObjectNTS( actualAttributeName );
    }
    else if( extensionPrefix == "TGS" )
    {
      attributeObject = createAttributeObjectTGS( actualAttributeName );
    }
    else if( extensionPrefix == "EGD" )
    {
      attributeObject = createAttributeObjectEGD( actualAttributeName );
    }
    else if( extensionPrefix == "SKL" )
    {
      attributeObject = createAttributeObjectSKL( actualAttributeName );
    }
    else if( extensionPrefix == "MPH" )
    {
      attributeObject = createAttributeObjectMPH( actualAttributeName );
    }
    else if( extensionPrefix == "SAS" )
    {
      attributeObject = createAttributeObjectSAS( actualAttributeName );
    }
    else if( extensionPrefix == "STI" )
    {
      attributeObject = createAttributeObjectSTI( actualAttributeName );
    }
    else if( extensionPrefix == "SYN" )
    {
      attributeObject = createAttributeObjectSYN( actualAttributeName );
    }
    else if( extensionPrefix == "AXN" )
    {
      attributeObject = createAttributeObjectAXN( actualAttributeName );
    }
    else if( extensionPrefix == "DEN" )
    {
      attributeObject = createAttributeObjectDEN( actualAttributeName );
    }
    else
    {
      qDebug( ) << "Unknown extension prefix.";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectDFL( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Name" ||
        actualAttributeName == "Alias" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      // Not needed.
      /**
      attributeObject.insert( "spatial", QJsonValue::fromVariant( false ) );
      attributeObject.insert( "key", QJsonValue::fromVariant( false ) );
      QJsonArray shapeArray;
      attributeObject.insert( "shape", shapeArray );
      attributeObject.insert( "continuous", QJsonValue::fromVariant( false ) );
      **/
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: Default. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectSEG( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Category" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Connections" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: Segmentation. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectISS( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Warning" ||
        actualAttributeName == "Critical" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SegmentationIssues. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectNTS( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Notes" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SegmentationNotes. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectTGS( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Tags" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SegmentationTags. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectEGD( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "LeftDistanceToEdge" ||
        actualAttributeName == "RightDistanceToEdge" ||
        actualAttributeName == "TopDistanceToEdge" ||
        actualAttributeName == "BottomDistanceToEdge" ||
        actualAttributeName == "FrontDistanceToEdge" ||
        actualAttributeName == "BackDistanceToEdge" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "TouchEdge" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: EdgeDistance. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectSKL( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "TotalLength" ||
        actualAttributeName == "ConnectedComponents" ||
        actualAttributeName == "NumberOfStrokes" ||
        actualAttributeName == "Cycles" ||
        actualAttributeName == "FeretDiameter" ||
        actualAttributeName == "NumberOfConnections" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Centroid" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VEC3" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "ComponentLengthVector" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "COMPLEX" ) );
      QJsonObject metaObject;
      QJsonObject formatObject;
      formatObject.insert( "number", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      formatObject.insert( "value", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      metaObject.insert( "format", formatObject );
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "StrokeLengthVector" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "COMPLEX" ) );
      QJsonObject metaObject;
      QJsonObject formatObject;
      formatObject.insert( "name", QJsonValue::fromVariant( "CATEGORICAL" ) );
      formatObject.insert( "value", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      metaObject.insert( "format", formatObject );
      attributeObject.insert( "meta", metaObject );

    }
    else if( actualAttributeName == "StrokeAngleVector" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "COMPLEX" ) );
      QJsonObject metaObject;
      QJsonObject formatObject;
      formatObject.insert( "aName", QJsonValue::fromVariant( "CATEGORICAL" ) );
      formatObject.insert( "bName", QJsonValue::fromVariant( "CATEGORICAL" ) );
      formatObject.insert( "value", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      metaObject.insert( "format", formatObject );
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Path" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "GEOMETRIC" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SkeletonInformation. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectMPH( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Size" ||
        actualAttributeName == "PhysicalSize" ||
        actualAttributeName == "FeretDiameter" ||
        actualAttributeName == "SurfaceArea" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Centroid" ||
             actualAttributeName == "BinaryPrincipalMoments" ||
             actualAttributeName == "EquivalentEllipsoidDiameter" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VEC3" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "BinaryPrincipalAxes" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "MAT3X3" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "MeshPath" ||
             actualAttributeName == "VolumePath" ||
             actualAttributeName == "MeshName" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "GEOMETRIC" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SkeletonInformation. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectSAS( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "Area" ||
        actualAttributeName == "Perimeter" ||
        actualAttributeName == "AreaRatio" ||
        actualAttributeName == "Mean_GaussCurvature" ||
        actualAttributeName == "StdDev_GaussCurvature" ||
        actualAttributeName == "Mean_MeanCurvature" ||
        actualAttributeName == "StdDev_MeanCurvature" ||
        actualAttributeName == "Mean_MinCurvature" ||
        actualAttributeName == "StdDev_MinCurvature" ||
        actualAttributeName == "Mean_MaxCurvature" ||
        actualAttributeName == "StdDev_MaxCurvature" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Synapse" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "Shape" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "ORDINAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: AppositionSurface. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectSTI( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "NameVector" ||
        actualAttributeName == "BoolVector" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "VECN" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: StereologicalInclusion. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectSYN( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "DendriteConnection" ||
        actualAttributeName == "SpineName" ||
        actualAttributeName == "BranchedSpine" ||
        actualAttributeName == "LocationInSpine" ||
        actualAttributeName == "AxonConnection" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "ConnectionType" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "ORDINAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SynapseConnections. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectAXN( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "ShaftLength" ||
        actualAttributeName == "SynapseNum" ||
        actualAttributeName == "EnPassantNum" ||
        actualAttributeName == "TerminalsNum" ||
        actualAttributeName == "SynapseLinearDensity" ||
        actualAttributeName == "SynapseSpinesNum" ||
        actualAttributeName == "SynapseShaftNum" ||
        actualAttributeName == "SpineShaftRatio" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: AxonInformation. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  QJsonObject DataSetListWidget::createAttributeObjectDEN( QString actualAttributeName )
  {
    QJsonObject attributeObject;

    if( actualAttributeName == "ShaftLenght" ||
        actualAttributeName == "AxonsNum" ||
        actualAttributeName == "AxonsExcitatoryNum" ||
        actualAttributeName == "AxonsInhibitoryNum" ||
        actualAttributeName == "ExcitatoryInhibitoryRatio" ||
        actualAttributeName == "SpinesNum" ||
        actualAttributeName == "CompleteSpinesNum" ||
        actualAttributeName == "TruncatedSpinesNum" ||
        actualAttributeName == "MonosynapticSpinesNum" ||
        actualAttributeName == "NonSynapticSpinesNum" ||
        actualAttributeName == "MultisinapticSpinesNum" ||
        actualAttributeName == "BranchedSpinesNum" ||
        actualAttributeName == "SpinesLinearDensity" ||
        actualAttributeName == "TotalSpinesLength" ||
        actualAttributeName == "MeanSpineLength" ||
        actualAttributeName == "MeanNearNeighbour" ||
        actualAttributeName == "SynapsesNum" ||
        actualAttributeName == "SynapsesOnSpinesNum" ||
        actualAttributeName == "SynapsesOnShaftNum" ||
        actualAttributeName == "SynapsesShaftsRatio" ||
        actualAttributeName == "SynapsesLinearDensity" ||
        actualAttributeName == "SynapsesShaftLinearDensity" ||
        actualAttributeName == "SynapsesSpineLinearDensity" ||
        actualAttributeName == "MeanSynapsesPerSpine" ||
        actualAttributeName == "SpineDataLength" ||
        actualAttributeName == "SpineDataSynapsesNum" ||
        actualAttributeName == "SpineDataAsymmetricNum" ||
        actualAttributeName == "SpineDataAsymmetricHeadNum" ||
        actualAttributeName == "SpineDataAsymmetricNeckNum" ||
        actualAttributeName == "SpineDataSymmetricNum" ||
        actualAttributeName == "SpineDataSymmetricHeadNum" ||
        actualAttributeName == "SpineDataSymmetricNeckNum" ||
        actualAttributeName == "SpineDataAxonsNum" ||
        actualAttributeName == "SpineDataInhibitoryAxonsNum" ||
        actualAttributeName == "SpineDataExcitatoryAxonsNum" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "QUANTITATIVE" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else if( actualAttributeName == "SpineDataNames" ||
             actualAttributeName == "SpineDataComplete" ||
             actualAttributeName == "SpineDataBranched" )
    {
      attributeObject.insert( "data_structure_type", QJsonValue::fromVariant( "NONE" ) );
      attributeObject.insert( "data_type", QJsonValue::fromVariant( "CATEGORICAL" ) );
      QJsonObject metaObject;
      attributeObject.insert( "meta", metaObject );
    }
    else
    {
      qDebug( ) << "Extension: SynapseConnections. Unknown attribute name: "
                << actualAttributeName << ".";
    }

    return attributeObject;
  }

  vishnucommon::Properties DataSetListWidget::segsJsonSchemaToSP1Properties( QString jsonSchema )
  {
    vishnucommon::Properties properties = std::vector< vishnucommon::PropertyPtr >( );

    QJsonDocument jsonSchemaDocument = QJsonDocument::fromJson( jsonSchema.toUtf8( ) );

    if( jsonSchemaDocument.isNull( ) )
    {
      std::cout << "Invalid JSON." << std::endl;
      return properties;
    }

    QJsonObject jsonSchemaObject;

    if( !jsonSchemaDocument.isObject( ) )
    {
      std::cout << "This JSON document is not an object." << std::endl;
      return properties;
    }

    jsonSchemaObject = jsonSchemaDocument.object( );

    QJsonObject attributes = jsonSchemaObject[ "attributes" ].toObject( );

    vishnucommon::PropertyPtr property;
    foreach( const QString& key, attributes.keys( ) )
    {
      std::string attributeName = key.toStdString( );
      // Feedback.
      //std::cout << "Attr. name: " << attributeName << std::endl;

      QJsonObject attributeProperties =
        attributes.value( key ).toObject( );

      QString attributeDataType =
        attributeProperties[ "data_type" ].toString( );
      // Feedback.
      //std::cout << "Attr. data type: " << attributeDataType.toStdString() << std::endl;
      vishnucommon::DataCategory dataCategory =
        vishnucommon::toDataCategory( attributeDataType.toStdString( ) );

      QString attributeDataStructureType =
        attributeProperties[ "data_structure_type" ].toString( );
      // Feedback.
      //std::cout << "Attr. data structure type: " << attributeDataStructureType.toStdString() << std::endl;
      vishnucommon::DataStructureType dataStructureType =
        vishnucommon::toDataStructureType( attributeDataStructureType.toStdString( ) );

      property = std::make_shared< vishnucommon::Property >(
        attributeName, dataCategory, dataStructureType );

      properties.push_back( property );
    }

    return properties;
  }

  /** END EspINA methods. **/

// -----------------------------------------------------------------------------

#endif
  void DataSetListWidget::createDataSetsFromJSON(
    DataSetWidgets& dataSetWidgets, const std::string& path )
  {
    vishnucommon::DataSetsPtr dataSets =
      vishnucommon::JSON::deserialize< vishnucommon::DataSets >( path );

    for ( const auto& dataSet : dataSets->getDataSets( ) )
    {
      createDataSetFromCSV( dataSetWidgets, dataSet->getPath( ),
        dataSet->getProperties( ) );
    }

    _propertyGroups = dataSets->getPropertyGroups( );

  }

  void DataSetListWidget::createDataSetFromCSV( DataSetWidgets& dataSetWidgets,
     const std::string& path, const vishnucommon::Properties& properties )
  {
    bool notUsedPath = true;
    for ( int i = 0; i < count( ); ++i )
    {
      DataSetWidgetPtr dataSetWidget = static_cast< DataSetWidgetPtr >(
        itemWidget( item( i ) ) );
      if ( dataSetWidget->getPath( ) == path )
      {
        QMessageBox::warning( this, APPLICATION_NAME,
          tr( "This file is already loaded." ) );
        notUsedPath = false;
        break;
      }
    }

    if ( notUsedPath )
    {
      vishnucommon::DataSetPtr dataSet( new vishnucommon::DataSet( path ) );

      if ( properties.size( ) == 0 ) //CSV
      {
        std::vector< std::string > headers =
          vishnucommon::Files::readCsvHeaders( path );

        for ( const auto& header : headers )
        {
          dataSet->addProperty( vishnucommon::PropertyPtr(
            new vishnucommon::Property( header, vishnucommon::DataCategory::Undefined,
            vishnucommon::DataStructureType::None ) ) );
        }
      }
      else //JSON or SEG
      {
        dataSet->setProperties( properties );
      }

      //Add to dataset
      DataSetWidgetPtr dataSetWidget( new DataSetWidget( dataSet ) );
      dataSetWidget->setListWidgetItem( new QListWidgetItem( this ) );

      QListWidgetItem* listWidgetItem = dataSetWidget->getListWidgetItem( );
      addItem( listWidgetItem );
      listWidgetItem->setSizeHint( dataSetWidget->sizeHint ( ) );
      setItemWidget( listWidgetItem, dataSetWidget );

      dataSetWidgets.emplace_back( dataSetWidget );
    }
  }

  DataSetWidgets DataSetListWidget::addDataSets(
    const std::vector< std::string >& dropped )
  {
    DataSetWidgets dataSetWidgets;

    QStringList filePaths;

    if ( !dropped.empty( ) )
    {
      for (unsigned int i = 0; i < dropped.size( ); ++i )
      {
        filePaths << QString::fromStdString( dropped.at( i ) );
      }
    }
    else
    {
      filePaths = QFileDialog::getOpenFileNames( this,
        QString( "Select files" ), QString( "" ),
        QString( "CSV Tabular Data Files(*.csv);;"
          "JSON Schema(*.json);;"
          "EspINA Session Files(*.seg)"
        ) );
    }
    if ( filePaths.isEmpty( ) )
    {
      return dataSetWidgets;
    }

    filePaths.removeDuplicates( );

    for ( int i = 0; i < filePaths.count( ); ++i )
    {
      QString qFilePath = filePaths.at( i );
      std::string filepath = qFilePath.toStdString( );

      std::string extension = vishnucommon::Strings::lower(
        QFileInfo( qFilePath ).completeSuffix( ).toStdString( ) );

      if ( extension == STR_EXT_SEG )
      {
#ifdef USE_ESPINA
        createDataSetsFromSEG( dataSetWidgets, filepath );
#else
        std::cout << "Warning: This version of Vishnu is not compatible with EspINA segmentation files." << std::endl;
#endif
      }
      else if ( extension == STR_EXT_JSON )
      {
        createDataSetsFromJSON( dataSetWidgets, filepath );
      }
      else if ( extension == STR_EXT_CSV )
      {
        createDataSetFromCSV( dataSetWidgets, filepath );
      }
    }

    return dataSetWidgets;
  }

  std::vector< std::string > DataSetListWidget::getPropertiesToRemove( )
  {
    DataSetWidgetPtr currentDsw = static_cast< DataSetWidgetPtr >(
      itemWidget( currentItem( ) ) );

    std::vector< std::string > propertiesToRemove =
      currentDsw->getDataSet( )->getPropertyNames( );

    for( int row = 0; row < count( ); ++row )
    {
      QListWidgetItem* listItem = item( row );
      if ( listItem != currentItem( ) )
      {
        DataSetWidgetPtr dsw = static_cast< DataSetWidgetPtr >(
          itemWidget( listItem ) );
        std::vector< std::string > properties =
          dsw->getDataSet( )->getPropertyNames( );
        for ( const auto& property: properties )
        {
          propertiesToRemove.erase( std::remove( propertiesToRemove.begin( ),
            propertiesToRemove.end( ), property ), propertiesToRemove.end( ) );
        }
      }
    }

    return propertiesToRemove;
  }

  void DataSetListWidget::removeCurrentDataSet( )
  {    
    takeItem( row( currentItem( ) ) );
  }

  vishnucommon::DataSetsPtr DataSetListWidget::getDataSets( void ) const
  {
    vishnucommon::DataSetsPtr dataSets( new vishnucommon::DataSets( ) );

    for( int row = 0; row < count( ); ++row )
    {
      DataSetWidgetPtr dsw = static_cast< DataSetWidgetPtr >(
        itemWidget( item( row ) ) );

      vishnucommon::DataSetPtr dataSet( new vishnucommon::DataSet( dsw->getPath( ) ) );
      dataSet->setProperties( dsw->getDataSet( )->getProperties( ) );
      dataSets->addDataSet( dataSet );
    }

    return dataSets;
  }

  vishnucommon::PropertyGroupsPtr DataSetListWidget::getPropertyGroups(
    void ) const
  {
    return _propertyGroups;
  }

  void DataSetListWidget::setPropertyGroups(
    const vishnucommon::PropertyGroupsPtr& propertyGroups )
  {
    _propertyGroups = propertyGroups;
  }

  std::vector< std::string > DataSetListWidget::getCommonProperties( )
  {
    std::vector< std::string > commonProperties;
    for ( const auto& dataset : getDataSets( )->getDataSets( ) )
    {
      std::vector< std::string > dataSetHeaders = dataset->getPropertyNames( );

      if ( !commonProperties.empty( ) )
      {
        commonProperties = vishnucommon::Vectors::intersect( commonProperties,
          dataSetHeaders );
      }
      else
      {
        commonProperties = dataSetHeaders;
      }
    }

    return commonProperties;
  }

  void DataSetListWidget::dragEnterEvent( QDragEnterEvent* event )
  {
    if ( event->mimeData()->hasUrls( ) )
    {
      event->acceptProposedAction();
    }
  }

  void DataSetListWidget::dragMoveEvent( QDragMoveEvent* event )
  {
    event->acceptProposedAction();
  }

  void DataSetListWidget::dragLeaveEvent( QDragLeaveEvent* event )
  {
    event->accept();
  }

  void DataSetListWidget::dropEvent( QDropEvent* event )
  {
    std::vector< std::string > files;
    for ( const QUrl& url : event->mimeData( )->urls( ) )
    {
      QString filePath = url.toLocalFile();

      std::string extension = vishnucommon::Strings::lower(
        QFileInfo( filePath ).completeSuffix( ).toStdString( ) );

      if ( vishnucommon::Vectors::find( { STR_EXT_CSV, STR_EXT_JSON, STR_EXT_SEG },
        extension ) != -1 )
      {
        files.emplace_back( filePath.toStdString( ) );
      }
    }
    if ( !files.empty( ) )
    {
      vishnucommon::Debug::consoleMessage( "Dropped files" );
      emit signalAddFiles( files );
    }
  }
}
