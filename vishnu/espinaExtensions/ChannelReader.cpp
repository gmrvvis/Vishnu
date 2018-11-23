/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín <juan.guerrero@upm.es>
 *          Félix de las Pozas Alvarez <fpozas@cesvima.upm.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#include "ChannelReader.h"

//#include <EspinaConfig.h>

// Qt.
#include <QString>
#include <QXmlStreamAttributes>

// EspINA.
#include <EspinaCore/Factory/CoreFactory.h>
#include <EspinaCore/Analysis/Channel.h>
#include <EspinaCore/Analysis/Sample.h>
#include <EspinaCore/Analysis/Output.h>
#include <EspinaCore/Analysis/Filters/VolumetricStreamReader.h>
#include <EspinaCore/Utils/EspinaException.h>
#include <EspinaCore/Utils/TemporalStorage.h>
#include <EspinaCore/IO/DataFactory/RawDataFactory.h>
#include <EspinaCore/IO/ProgressReporter.h>

/**
#if USE_METADONA
  #include <Producer.h>
  #include <IRODS_Storage.h>
  #include <Support/Metadona/Coordinator.h>
  #include <Support/Metadona/StorageFactory.h>
#endif
**/

namespace vishnu
{

const ESPINA::Filter::Type ChannelReader::VOLUMETRIC_STREAM_READER    = "ChannelReader::VolumetricStreamReader";
const ESPINA::Filter::Type ChannelReader::ESPINA_1_3_2_CHANNEL_READER = "Channel Reader";

class UpdateFilterDataFactory
: public ESPINA::DataFactory
{
  public:
    virtual ESPINA::DataSPtr createData(ESPINA::OutputSPtr output,
                                        ESPINA::TemporalStorageSPtr storage,
                                        const QString& path,
                                        QXmlStreamAttributes info) override
    {
      ESPINA::DataSPtr data;

      if ("VolumetricData" == info.value("type"))
      {
        data = fetchData(output);
      }
      else
      {
        data = m_fetchData.createData(output, storage, path, info);
      }

      return data;
    }

  private:
    ESPINA::DefaultVolumetricDataSPtr fetchData(ESPINA::OutputSPtr output)
    {
      output->filter()->update();
      return ESPINA::writeLockVolume(output, ESPINA::DataUpdatePolicy::Ignore);
    }

    ESPINA::RawDataFactory m_fetchData;
};

class DummyReporter
: public ESPINA::IO::ProgressReporter
{
  public:
    DummyReporter( void )
    { };

    virtual ~DummyReporter( void )
    { };

    virtual void setProgress(unsigned int progress)
    {
      std::cout << progress << std::endl;
    }
};

//------------------------------------------------------------------------
ESPINA::FilterTypeList ChannelReader::providedFilters() const
{
  ESPINA::FilterTypeList filters;

  filters << VOLUMETRIC_STREAM_READER << ESPINA_1_3_2_CHANNEL_READER;

  return filters;
}

//------------------------------------------------------------------------
ESPINA::FilterSPtr ChannelReader::createFilter(ESPINA::InputSList inputs,
                                               const ESPINA::Filter::Type& filter,
                                               ESPINA::SchedulerSPtr scheduler) const
{
  static auto dataFactory = std::make_shared<UpdateFilterDataFactory>();

  if (filter != VOLUMETRIC_STREAM_READER && filter != ESPINA_1_3_2_CHANNEL_READER)
  {
    auto what    = QObject::tr("Unable to create filter: %1").arg(filter);
    auto details = QObject::tr("ChannelReader::createFilter() -> Unknown filter type: %1").arg(filter);
    throw ESPINA::Core::Utils::EspinaException(what, details);
  }

  auto reader = std::make_shared<ESPINA::Core::VolumetricStreamReader>(inputs, VOLUMETRIC_STREAM_READER, scheduler);
  reader->setDataFactory(dataFactory); //FIX: Temporal fix to create output during seg file load

  return reader;
}

//------------------------------------------------------------------------
ESPINA::IO::AnalysisReader::ExtensionList ChannelReader::supportedFileExtensions() const
{
  ESPINA::IO::AnalysisReader::ExtensionList supportedExtensions;

  ESPINA::IO::AnalysisReader::Extensions extensions;
  extensions << "mha" << "mhd" << "tiff" << "tif";

  supportedExtensions["Channel Files"] = extensions;

  return supportedExtensions;
}

//------------------------------------------------------------------------
ESPINA::AnalysisSPtr ChannelReader::read(const QFileInfo& file,
                                         ESPINA::CoreFactorySPtr  factory,
                                         ESPINA::IO::ProgressReporter *reporter,
                                         ESPINA::ErrorHandlerSPtr handler)
{
  auto analysis = std::make_shared<ESPINA::Analysis>();

  analysis->setStorage(factory->createTemporalStorage());

  auto sampleName = QString("Unknown Sample");

  QString channelMetadata;

/**
#if USE_METADONA
  if(!StorageFactory::supportedStorages().isEmpty())
  {
    Coordinator coordinator;

    auto storage = StorageFactory::newStorage(StorageFactory::Type::IRODS);

    Metadona::Producer producer(storage);

    try
    {
      auto metadata = storage->metadata(file.absoluteFilePath().toStdString());

      if (metadata.empty())
      {
        metadata = producer.generateFrom("Specimen", coordinator);

        storage->setMetadata(file.absoluteFilePath().toStdString(), metadata);
      }

      std::cout << Metadona::dump(metadata) << std::endl;
      channelMetadata = Metadona::dump(metadata).c_str();

      sampleName = metadata.at(0).id().c_str();
    }
    catch(...)
    {

    }
  }
#endif
**/

  auto sample = factory->createSample(sampleName);

  analysis->add(sample);

  auto filter = factory->createFilter<ESPINA::Core::VolumetricStreamReader>(ESPINA::InputSList(), VOLUMETRIC_STREAM_READER);
  filter->setErrorHandler(handler);
  filter->setFileName(file);
  filter->setStorage(factory->defaultStorage());
  filter->update();

  auto channel = factory->createChannel(filter, 0);
  channel->setName(file.fileName());
  channel->setMetadata(channelMetadata);

  analysis->add(channel);

  analysis->addRelation(sample, channel, ESPINA::Channel::STAIN_LINK);

  reporter = new DummyReporter( );
  reporter->setProgress( 100 );

  return analysis;
}

} // namespace vishnu
