/*
 * Copyright (c) 2017-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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

#ifndef VISHNU_APPPROCESS_H
#define VISHNU_APPPROCESS_H

#include <QProcess>
#include <QPushButton>

#include <string>

#include <vishnucommon/vishnucommon.h>

#include "model/Application.h"

namespace vishnu
{
  class AppProcess : public QProcess, public Application
  {
    public:

      AppProcess( const vishnucommon::ApplicationType& applicationType,
        const std::string& displayName, const std::string& shellCommand,
        const vishnucommon::Args& args, const std::string& workingDirectory,
        const std::string& iconPath );

      ~AppProcess( );

      QPushButton* getPushButton( ) const;

    private:

      QPushButton* _pushButton;
  };
}

#endif
