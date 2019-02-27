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
