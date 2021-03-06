/*
 * Vishnu
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín <juan.guerrero@upm.es>
 *          Félix de las Pozas Alvarez <fpozas@cesvima.upm.es>
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

#ifndef VISHNU_EXTENSIONINFORMATIONDUMP_H
#define VISHNU_EXTENSIONINFORMATIONDUMP_H

// std.
#include <map>

// Qt.
#include <QString>

// EspINA.
#include <EspinaCore/Analysis/Extensible.hxx>
#include <EspinaCore/Analysis/Extensions.h>
#include <EspinaCore/Analysis/Segmentation.h>
#include <EspinaCore/Types.h>

// Only needed for spine information.
#include <EspinaExtensions/SkeletonInformation/DendriteInformation.h>

namespace vishnu
{

  /** \brief Returns a row of the given segmentation information for the given extension and keys for a CSV file.
   * \param[in] segmentation Segmentation object pointer.
   * \param[in] type Extension type identifier.
   * \param[in] keys Keys for the given extension type.
   *
   */
  const QString dumpExtensionInformationToCSV(const ESPINA::SegmentationPtr                       segmentation,
                                              const ESPINA::Core::SegmentationExtension::Type    &type,
                                              const ESPINA::Core::SegmentationExtension::KeyList &keys);

  /** \brief Dumps the header of the CSV table for the given type and keys.
   * \param[in] type Extension type identifier.
   * \param[in] keys Keys for the given extension type.
   *
   */
  const QString dumpExtensionHeaderToCSV(const ESPINA::Core::SegmentationExtension::Type    &type,
                                         const ESPINA::Core::SegmentationExtension::KeyList &keys);

} // namespace vishnu

#endif
