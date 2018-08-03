/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_APPPROCESS_H
#define VISHNU_APPPROCESS_H

#include <QProcess>
#include <QPushButton>

#include <string>

#include <sp1common/sp1common.h>

#include "model/Application.h"

namespace vishnu
{
  class AppProcess : public QProcess, public Application
  {
    public:

      AppProcess( const sp1common::ApplicationType& applicationType,
        const std::string& displayName, const std::string& shellCommand,
        const sp1common::Args& args, const std::string& workingDirectory,
        const std::string& iconPath );

      ~AppProcess( );

      QPushButton* getPushButton( ) const;

    private:
      
      QPushButton* _pushButton;
  };
}

#endif
