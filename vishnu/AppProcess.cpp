/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#include "AppProcess.h"

namespace vishnu
{
  AppProcess::AppProcess( const sp1common::ApplicationType& applicationType,
    const std::string& displayName, const std::string& shellCommand,
    const sp1common::Args& args, const std::string& workingDirectory,
    const std::string& iconPath )
      : Application( applicationType, displayName, shellCommand, args,
        workingDirectory, iconPath )
  {
    _pushButton = new QPushButton( QString::fromStdString( displayName ) );
    _pushButton->setStyleSheet(
      "text-align:left; font-weight: bold; font-size: 18px" );
    _pushButton->setIcon( QIcon( QString::fromStdString( iconPath ) ) );
    _pushButton->setIconSize( QSize( 64, 64 ) );
    _pushButton->setSizePolicy( QSizePolicy::Preferred,
      QSizePolicy::Preferred );
  }

  AppProcess::~AppProcess( )
  {

  }

  QPushButton* AppProcess::getPushButton( ) const
  {
    return _pushButton;
  }
}
