/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín <juan.guerrero@upm.es>
 *          Félix de las Pozas Alvarez <fpozas@cesvima.upm.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/
#ifndef VISHNU_CHANNELREADER_H
#define VISHNU_CHANNELREADER_H

// Qt.
#include <QFileInfo>

// EspINA.
#include <EspinaCore/Factory/AnalysisReader.h>
#include <EspinaCore/Factory/FilterFactory.h>

namespace vishnu
{
  /** \class ChannelReader
   * \brief Implements a reader for stacks.
   *
   */
  class ChannelReader
  : public ESPINA::FilterFactory
  , public ESPINA::IO::AnalysisReader
  {
    public:
      static const ESPINA::Filter::Type VOLUMETRIC_STREAM_READER;    /** channel reader signature. */
      static const ESPINA::Filter::Type ESPINA_1_3_2_CHANNEL_READER; /** channel reader old signature. */

      virtual QString type() const
      { return "ChannelReader"; }

      virtual ESPINA::FilterTypeList providedFilters() const;

      virtual ESPINA::FilterSPtr createFilter(ESPINA::InputSList inputs, const ESPINA::Filter::Type& filter, ESPINA::SchedulerSPtr scheduler) const;

      virtual ESPINA::IO::AnalysisReader::ExtensionList supportedFileExtensions() const;

      virtual ESPINA::AnalysisSPtr read(const QFileInfo& file,
                                        ESPINA::CoreFactorySPtr  factory,
                                        ESPINA::IO::ProgressReporter *reporter = nullptr,
                                        ESPINA::ErrorHandlerSPtr handler = ESPINA::ErrorHandlerSPtr());
  };

  using ChannelReaderSPtr = std::shared_ptr<ChannelReader>;

} // namespace vishnu

#endif
