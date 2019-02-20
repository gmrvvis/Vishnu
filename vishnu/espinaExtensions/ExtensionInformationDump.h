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
