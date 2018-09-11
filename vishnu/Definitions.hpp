/**
 * Copyright (c) 2017-2018 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
 *
 * This file is part of Vishnu <https://gitlab.gmrv.es/cbbsp1/vishnu>
*/

#ifndef VISHNU_DEFINITIONS_HPP
#define VISHNU_DEFINITIONS_HPP

#define APPLICATION_NAME "Vishnu"
#define ORGANIZATION_NAME "GMRV - URJC"

#define APPLICATION_WIDTH 1366
#define APPLICATION_HEIGHT 768

#define USER_DATA_FOLDER "userdata/"
#define GEOMETRY_DATA_FOLDER "geometricData/"
#define DEFAULT_DATASET_FILENAME "dataSet"
#define FILE_USER_PREFERENCES "UserPreferences.json"
#define FILE_APPS_CONFIG "AppsConfig.json"
#define FILE_DATASETS "DataSets.json"

#define MAX_DATASET_NAME_LENGTH 10

#if defined(_WIN32) || defined(WIN32)
  #define SUPERUSER "powershell -Command \"Start-Process cmd -Verb -RunAs\""
#else
  #define SUPERUSER "gksudo"
#endif

#define STR_EXT_CSV "csv"
#define STR_EXT_JSON "json"
#define STR_EXT_SEG "seg"
#define STR_EXT_XML "xml"

#define MISSING_DATA_FIELD "#!#Missing Data#!#"

#endif
