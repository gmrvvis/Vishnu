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

#ifndef VISHNU_DEFINITIONS_HPP
#define VISHNU_DEFINITIONS_HPP

#define APPLICATION_NAME "Vishnu"
#define ORGANIZATION_NAME "GMRV - URJC"

#define APPLICATION_WIDTH 1366
#define APPLICATION_HEIGHT 768

#define STR_ZEQSESSION "zeqSession"
#define STR_WORKINGDIRECTORY "workingDirectory"

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
