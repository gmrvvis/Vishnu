/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín <juan.guerrero@upm.es>
 *          Félix de las Pozas Alvarez <fpozas@cesvima.upm.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "ExtensionInformationDump.h"

namespace vishnu
{

  // Types of supported segmentation extensions. Needs to be updated and in sync with SegmentationExtension::TYPE
  // Could use SegmentationExtension::TYPE but _will_ require linking to all plugins that implement an extension, this is just to avoid it.
  const QString SegmentationIssuesType      {"SegmentationIssues"};
  const QString SegmentationNotesType       {"SegmentationNotes"};
  const QString SegmentationTagsType        {"SegmentationTags"};
  const QString EdgeDistancesType           {"EdgeDistance"};
  const QString MorphologicalInformationType{"MorphologicalInformation"};
  const QString SkeletonInformationType     {"SkeletonInformation"};
  const QString AppositionSurfaceType       {"AppositionSurface"};
  const QString StereologicalInclusionType  {"StereologicalInclusion"};
  const QString SynapseConnectionType       {"SynapseConnections"};
  const QString AxonInformationType         {"AxonInformation"};
  const QString DendriteInformationType     {"DendriteInformation"};

  // Keys translations between EspINA extensions and Vishnu. Needs to be updated and in sync.
  std::map<const QString, const QString> SegmentationNotesTranslations   = { { "Notes",                                          "NTSNotes"} };
  std::map<const QString, const QString> SegmentationTagsTranslations    = { { "Tags",                                           "TGSTags" } };
  std::map<const QString, const QString> EdgeDistanceTranslations        = { { "Left Distance",                                  "EGDLeftDistanceToEdge" },
                                                                             { "Top Distance",                                   "EGDTopDistanceToEdge" },
                                                                             { "Front Distance",                                 "EGDFrontDistanceToEdge" },
                                                                             { "Right Distance",                                 "EGDRightDistanceToEdge" },
                                                                             { "Bottom Distance",                                "EGDBottomDistanceToEdge" },
                                                                             { "Back Distance",                                  "EGDBackDistanceToEdge" },
                                                                             { "Touch Edge",                                     "EGDTouchEdge" } };
  std::map<const QString, const QString> MorphologicalTranslations       = { { "Size",                                           "MPHSize" },
                                                                             { "Physical Size",                                  "MPHPhysicalSize" },
                                                                             { "Centroid X",                                     "MPHCentroid" },
                                                                             { "Centroid Y",                                     "" },
                                                                             { "Centroid Z",                                     "" },
                                                                             { "Binary Principal Moments X",                     "MPHBinaryPrincipalMoments" },
                                                                             { "Binary Principal Moments Y",                     "" },
                                                                             { "Binary Principal Moments Z",                     "" },
                                                                             { "Binary Principal Axes (0 0)",                    "MPHBinaryPrincipalAxes" },
                                                                             { "Binary Principal Axes (0 1)",                    "" },
                                                                             { "Binary Principal Axes (0 2)",                    "" },
                                                                             { "Binary Principal Axes (1 0)",                    "" },
                                                                             { "Binary Principal Axes (1 1)",                    "" },
                                                                             { "Binary Principal Axes (1 2)",                    "" },
                                                                             { "Binary Principal Axes (2 0)",                    "" },
                                                                             { "Binary Principal Axes (2 1)",                    "" },
                                                                             { "Binary Principal Axes (2 2)",                    "" },
                                                                             { "Feret Diameter",                                 "MPHFeretDiameter" },
                                                                             { "Equivalent Ellipsoid Diameter X",                "MPHEquivalentEllipsoidDiameter" },
                                                                             { "Equivalent Ellipsoid Diameter Y",                "" },
                                                                             { "Equivalent Ellipsoid Diameter Z",                "" },
                                                                             { "Surface Area",                                   "MPHSurfaceArea" },
                                                                             { "dummy1",                                         "MPHVolumePath"},
                                                                             { "dummy2",                                         "MPHMeshPath"},
                                                                             { "dummy3",                                         "MPHMeshName"} };
  std::map<const QString, const QString> AppositionSurfaceTranslations   = { { "Area",                                           "SASArea" },
                                                                             { "Perimeter",                                      "SASPerimeter" },
                                                                             { "Area Ratio",                                     "SASAreaRatio" },
                                                                             { "Synapse",                                        "SASSynapse" },
                                                                             { "Shape",                                          "SASShape" },
                                                                             { "Mean Gauss Curvature",                           "SASMean_GaussCurvature" },
                                                                             { "Std Deviation Gauss Curvature",                  "SASStdDev_GaussCurvature" },
                                                                             { "Mean Mean Curvature",                            "SASMean_MeanCurvature" },
                                                                             { "Std Deviation Mean Curvature",                   "SASStdDev_MeanCurvature" },
                                                                             { "Mean Min Curvature",                             "SASMean_MinCurvature" },
                                                                             { "Std Deviation Min Curvature",                    "SASStdDev_MinCurvature" },
                                                                             { "Mean Max Curvature",                             "SASMean_MaxCurvature" },
                                                                             { "Std Deviation Max Curvature",                    "SASStdDev_MaxCurvature" } };
  std::map<const QString, const QString> StereologicalIncTranslations    = { { "dummy1",                                         "STINameVector"   },
                                                                             { "dummy2",                                         "STIBoolVector"   } };
  std::map<const QString, const QString> SegmentationIssueTranslations   = { { } };
  std::map<const QString, const QString> SynapseConnectionTranslations   = { { "Dendrite connection",                            "SYNDendriteConnection" },
                                                                             { "Shaft/Spine",                                    "SYNConnectionType" },
                                                                             { "Spine name",                                     "SYNSpineName" },
                                                                             { "Branched spine",                                 "SYNBranchedSpine" },
                                                                             { "Location in spine",                              "SYNLocationInSpine" },
                                                                             { "Axon connection",                                "SYNAxonConnection" } };
  std::map<const QString, const QString> AxonInformationTranslations     = { { "Shaft length (Nm)",                              "AXNShaftLength"},
                                                                             { "Num of synapses",                                "AXNSynapseNum" },
                                                                             { "Num of synapses en passant",                     "AXNEnPassantNum"},
                                                                             { "Num of synapses on axon terminals",              "AXNTerminalsNum"},
                                                                             { "Linear density of synapses",                     "AXNSynapseLinearDensity"},
                                                                             { "Num of synapses on dendritic spines",            "AXNSynapseSpinesNum"},
                                                                             { "Num of synapses on dendritic shafts",            "AXNSynapseShaftNum"},
                                                                             { "Ratio of synapses on spines and shafts",         "AXNSpineShaftRatio"} };
  std::map<const QString, const QString> DendriteInformationTranslations = { { "Shaft Length (Nm)",                              "DENShaftLenght"},
                                                                             { "Num of axons contacted",                         "DENAxonsNum"},
                                                                             { "Num of excitatory axons contacted",              "DENAxonsExcitatoryNum"},
                                                                             { "Num of inhibitory axons contacted",              "DENAxonsInhibitoryNum"},
                                                                             { "Excitatory/inhibitory synapses ratio",           "DENExcitatoryInhibitoryRatio"},
                                                                             { "Num of spines",                                  "DENSpinesNum"},
                                                                             { "Num of complete spines",                         "DENCompleteSpinesNum"},
                                                                             { "Num of truncated spines",                        "DENTruncatedSpinesNum"},
                                                                             { "Num of monosynaptic spines",                     "DENMonosynapticSpinesNum"},
                                                                             { "Num of non-synaptic spines",                     "DENNonSynapticSpinesNum"},
                                                                             { "Num of multisynaptic spines",                    "DENMultisinapticSpinesNum"},
                                                                             { "Num of branched spines",                         "DENBranchedSpinesNum"},
                                                                             { "Linear density of spines",                       "DENSpinesLinearDensity"},
                                                                             { "Total length of complete spines (Nm)",           "DENTotalSpinesLength"},
                                                                             { "Mean spine length (Nm)",                         "DENMeanSpineLength"},
                                                                             { "Mean nearest neighbour distance of spines (Nm)", "DENMeanNearNeighbour"},
                                                                             { "Num of synapses",                                "DENSynapsesNum"},
                                                                             { "Num of synapses on spines",                      "DENSynapsesOnSpinesNum"},
                                                                             { "Num of synapses on shaft",                       "DENSynapsesOnShaftNum"},
                                                                             { "Ratio of synapses on spines and shafts",         "DENSynapsesShaftsRatio"},
                                                                             { "Linear density of synapses",                     "DENSynapsesLinearDensity"},
                                                                             { "Linear density of synapses on shaft",            "DENSynapsesShaftLinearDensity"},
                                                                             { "Linear density of synapses on spines",           "DENSynapsesSpineLinearDensity"},
                                                                             { "Mean Num of Synapses per spine",                 "DENMeanSynapsesPerSpine"},
                                                                             { "dummy01",                                        "DENSpineDataNames"},
                                                                             { "dummy02",                                        "DENSpineDataComplete"},
                                                                             { "dummy03",                                        "DENSpineDataBranched"},
                                                                             { "dummy04",                                        "DENSpineDataLength"},
                                                                             { "dummy05",                                        "DENSpineDataSynapsesNum"},
                                                                             { "dummy06",                                        "DENSpineDataAsymmetricNum"},
                                                                             { "dummy07",                                        "DENSpineDataAsymmetricHeadNum"},
                                                                             { "dummy08",                                        "DENSpineDataAsymmetricNeckNum"},
                                                                             { "dummy09",                                        "DENSpineDataSymmetricNum"},
                                                                             { "dummy10",                                        "DENSpineDataSymmetricHeadNum"},
                                                                             { "dummy11",                                        "DENSpineDataSymmetricNeckNum"},
                                                                             { "dummy12",                                        "DENSpineDataAxonsNum"},
                                                                             { "dummy13",                                        "DENSpineDataInhibitoryAxonsNum"},
                                                                             { "dummy14",                                        "DENSpineDataExcitatoryAxonsNum"} };

  //--------------------------------------------------------------------
  const QString dumpExtensionInformationToCSV(const ESPINA::SegmentationPtr                       segmentation,
                                              const ESPINA::Core::SegmentationExtension::Type    &type,
                                              const ESPINA::Core::SegmentationExtension::KeyList &keys)
  {
    const QString separator{","};
    const QString concatenator{";"};
    QString result;

    auto concatenateInformationValues = [segmentation, type, concatenator] (const ESPINA::Core::SegmentationExtension::KeyList &keys_)
    {
      QString result_;

      for(auto key: keys_)
      {
        auto iKey_ = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey_))
        {
          result_ += segmentation->information(iKey_).toString().simplified();

          if(key != keys_.last()) result_ += concatenator;
        }
      }

      return result_;
    };

    if(type == SegmentationIssuesType)
    {
      // Nothing to do, these warnings are only for EspINA users.
      return result;
    }

    // Extension: MPH.
    if(type == MorphologicalInformationType)
    {
      QStringList banned = { "Centroid Y", "Centroid Z", "Binary Principal Moments Y", "Binary Principal Moments Z",
                             "Binary Principal Axes (0 1)", "Binary Principal Axes (0 2)", "Binary Principal Axes (1 0)",
                             "Binary Principal Axes (1 1)", "Binary Principal Axes (1 2)", "Binary Principal Axes (2 0)",
                             "Binary Principal Axes (2 1)", "Binary Principal Axes (2 2)", "Equivalent Ellipsoid Diameter Y",
                             "Equivalent Ellipsoid Diameter Z"};

      for(auto key: keys)
      {
        if(banned.contains(key)) continue;

        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};

        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          if(key.startsWith("Centroid", Qt::CaseInsensitive))
          {
            const ESPINA::Core::SegmentationExtension::KeyList aggregate = { "Centroid X", "Centroid Y", "Centroid Z" };
            result += concatenateInformationValues(aggregate);
          }
          else if(key.startsWith("Binary Principal Moments", Qt::CaseInsensitive))
          {
            const ESPINA::Core::SegmentationExtension::KeyList aggregate = { "Binary Principal Moments X", "Binary Principal Moments Y", "Binary Principal Moments Z" };
            result += concatenateInformationValues(aggregate);
          }
          else if(key.startsWith("Binary Principal Axes", Qt::CaseInsensitive))
          {
            const ESPINA::Core::SegmentationExtension::KeyList aggregate = { "Binary Principal Axes (0 0)", "Binary Principal Axes (0 1)", "Binary Principal Axes (0 2)",
                                                                             "Binary Principal Axes (1 0)", "Binary Principal Axes (1 1)", "Binary Principal Axes (1 2)",
                                                                             "Binary Principal Axes (2 0)", "Binary Principal Axes (2 1)", "Binary Principal Axes (2 2)"};
            result += concatenateInformationValues(aggregate);
          }
          else if(key.startsWith("Equivalent Ellipsoid Diameter", Qt::CaseInsensitive))
          {
            const ESPINA::Core::SegmentationExtension::KeyList aggregate = { "Equivalent Ellipsoid Diameter X", "Equivalent Ellipsoid Diameter Y", "Equivalent Ellipsoid Diameter Z" };
            result += concatenateInformationValues(aggregate);
          }
          else
          {
            auto iKeyAux = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
            result += segmentation->information(iKeyAux).toString().simplified();
          }
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (MorphologicalInformationType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        result += separator;
      }

      // add non standard keys MPHVolumePath & MPHMeshPath
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
      result += QString("/geometricData/%1-%2-volume.json").arg( segNameUScore ).arg( segAliasUScore ) + separator +
                QString("/geometricData/%1-%2-mesh.obj").arg( segNameUScore ).arg( segAliasUScore ) + separator +
                QString("%1-%2-mesh").arg( segNameUScore ).arg( segAliasUScore );

      return result;
    }

    // Extension: EGD.
    if(type == EdgeDistancesType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};

        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          auto keyInformation = segmentation->information(iKey);
          if( keyInformation.isValid( ) )
          {
            result += keyInformation.toString().simplified();
          }
          else
          {
            std::cout << "The key " << iKey.value().toStdString() << " has no value." << std::endl;
          }
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (EdgeDistancesType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: NTS.
    if(type == SegmentationNotesType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (SegmentationNotesType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: TGS.
    if(type == SegmentationTagsType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (SegmentationTagsType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: SAS.
    if(type == AppositionSurfaceType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (AppositionSurfaceType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: STI.
    if(type == StereologicalInclusionType)
    {
      QString cfs;
      QString values;

      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          auto name = key.split(' ');
          Q_ASSERT(name.size() == 3);
          cfs    += name.at(1);
          values += (segmentation->information(iKey).toBool() ? "true" : "false");

          if(key != keys.last())
          {
            cfs    += concatenator;
            values += concatenator;
          }
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (StereologicalInclusionType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/
      }

      result += cfs + separator + values;

      return result;
    }

    // Extension: SYN.
    if(type == SynapseConnectionType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (SynapseConnectionType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: AXN.
    if(type == AxonInformationType)
    {
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (AxonInformationType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: DEN.
    if(type == DendriteInformationType)
    {
      bool anyInformation = false;
      for(auto key: keys)
      {
        auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
        if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
        {
          result += segmentation->information(iKey).toString().simplified();
          anyInformation = true;
        }

        // Feedback.
        /**
        else
        {
          std::cout << "Segmentation (" << segmentation->name().toStdString()
                    << ") has NOT (DendriteInformationType) this information: "
                    << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
        }
        **/

        if(key != keys.last()) result += separator;
      }

      // Separating sections.
      result += separator;

      // Spine data.
      /**/
      if( anyInformation )
      {
        auto extension = segmentation->readOnlyExtensions()->get< ESPINA::Extensions::DendriteSkeletonInformation >( );

        // Temporary solution.
        if( extension == nullptr )
        {
          std::cout << "DendriteSkeletonInformation NOT available." << std::endl;

          result += separator + separator + separator + separator + separator +
            separator + separator + separator + separator + separator +
            separator + separator + separator;
        }
        else
        {
          std::cout << "DendriteSkeletonInformation available." << std::endl;

          auto table = extension->spinesInformation();

          QStringList nameList, completeList, branchedList, lengthList, numSynapsesList, numAsymmetricList, numAsymmetricHeadList,
          numAsymmetricNeckList, numSymmetricList, numSymmetricHeadList, numSymmetricNeckList, numAxonsList, numInhibitoryAxonsList,
          numExcitatoryAxonsList;

          for(auto data: table)
          {
            nameList               << data.name.simplified();
            completeList           << (data.complete ? "true" : "false");
            branchedList           << (data.branched ? "yes" : "no");
            lengthList             << QString::number(data.length);
            numSynapsesList        << QString::number(data.numSynapses);
            numAsymmetricList      << QString::number(data.numAsymmetric);
            numAsymmetricHeadList  << QString::number(data.numAsymmetricHead);
            numAsymmetricNeckList  << QString::number(data.numAsymmetricNeck);
            numSymmetricList       << QString::number(data.numSymmetric);
            numSymmetricHeadList   << QString::number(data.numSymmetricHead);
            numSymmetricNeckList   << QString::number(data.numSymmetricNeck);
            numAxonsList           << QString::number(data.numAxons);
            numInhibitoryAxonsList << QString::number(data.numAxonsInhibitory);
            numExcitatoryAxonsList << QString::number(data.numAxonsExcitatory);
          }

          result += nameList.join(";")               + separator;
          result += completeList.join(";")           + separator;
          result += branchedList.join(";")           + separator;
          result += lengthList.join(";")             + separator;
          result += numSynapsesList.join(";")        + separator;
          result += numAsymmetricList.join(";")      + separator;
          result += numAsymmetricHeadList.join(";")  + separator;
          result += numAsymmetricNeckList.join(";")  + separator;
          result += numSymmetricList.join(";")       + separator;
          result += numSymmetricHeadList.join(";")   + separator;
          result += numSymmetricNeckList.join(";")   + separator;
          result += numAxonsList.join(";")           + separator;
          result += numInhibitoryAxonsList.join(";") + separator;
          result += numExcitatoryAxonsList.join(";");
        }
      }
      else
      {
        result += separator + separator + separator + separator + separator +
          separator + separator + separator + separator + separator +
          separator + separator + separator;
      }
      /**/

      return result;
    }

    // Extension: Unknown.
    for(auto key: keys)
    {
      auto iKey = ESPINA::Core::SegmentationExtension::InformationKey{type, key};
      if(segmentation->readOnlyExtensions()->availableInformation().contains(iKey))
      {
        result += segmentation->information(iKey).toString().simplified();
      }

      // Feedback.
      /**
      else
      {
        std::cout << "Segmentation (" << segmentation->name().toStdString()
                  << ") has NOT (Others) this information: "
                  << type.toStdString() << " > " << key.toStdString( ) << "." << std::endl;
      }
      **/

      if(key != keys.last()) result += separator;
    }

    return result;
  }

  //--------------------------------------------------------------------
  const QString dumpExtensionHeaderToCSV(const ESPINA::Core::SegmentationExtension::Type    &type,
                                         const ESPINA::Core::SegmentationExtension::KeyList &keys)
  {
    const QString separator{","};
    const QString concatenator{";"};
    QString result;

    if(type == SegmentationIssuesType)
    {
      // not exported
      return result;
    }

    // Extension: MPH.
    if(type == MorphologicalInformationType)
    {
      for(auto key: keys)
      {
        auto value = MorphologicalTranslations.at(key);

        if(value.isEmpty()) continue;

        result += value + separator;
      }

      result += MorphologicalTranslations.at("dummy1") + separator +
                MorphologicalTranslations.at("dummy2") + separator +
                MorphologicalTranslations.at("dummy3");

      return result;
    }

    // Extension: EGD.
    if(type == EdgeDistancesType)
    {
      for(auto key: keys)
      {
        result += EdgeDistanceTranslations.at(key);

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: NTS.
    if(type == SegmentationNotesType)
    {
      for(auto key: keys)
      {
        result += SegmentationNotesTranslations.at(key);
      }

      return result;
    }

    // Extension: TGS.
    if(type == SegmentationTagsType)
    {
      for(auto key: keys)
      {
        result += SegmentationTagsTranslations.at(key);
      }

      return result;
    }

    // Extension: SAS.
    if(type == AppositionSurfaceType)
    {
      for(auto key: keys)
      {
        result += AppositionSurfaceTranslations.at(key);

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: STI.
    if(type == StereologicalInclusionType)
    {
      for(auto it = StereologicalIncTranslations.begin(); it != StereologicalIncTranslations.end();)
      {
        result += it->second;
        ++it;

        if(it != StereologicalIncTranslations.end())
        {
          result += separator;
        }
      }

      return result;
    }

    // Extension: SYN.
    if(type == SynapseConnectionType)
    {
      for(auto key: keys)
      {
        result += SynapseConnectionTranslations.at(key);

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: AXN.
    if(type == AxonInformationType)
    {
      for(auto key: keys)
      {
        result += AxonInformationTranslations.at(key);

        if(key != keys.last()) result += separator;
      }

      return result;
    }

    // Extension: DEN.
    if(type == DendriteInformationType)
    {
      for(auto key: keys)
      {
        result += DendriteInformationTranslations.at(key) + separator;
      }

      // Spine data.
      /**/
      QStringList spineKeys{"dummy01", "dummy02", "dummy03", "dummy04", "dummy05", "dummy06", "dummy07",
                            "dummy08", "dummy09", "dummy10", "dummy11", "dummy12", "dummy13", "dummy14"};
      for(auto spineKey: spineKeys)
      {
        result += DendriteInformationTranslations.at(spineKey);

        if(spineKey != spineKeys.last()) result += separator;
      }
      /**/

      return result;
    }

    // Extension: Unknown.
    for(auto key: keys)
    {
      result += "UNK" + key;

      if(key != keys.last()) result += separator;
    }

    return result;
  }

} // namespace vishnu
