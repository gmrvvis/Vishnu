#ifndef VISHNU_DEFINITIONS_HPP
#define VISHNU_DEFINITIONS_HPP

#define APPLICATION_NAME "Vishnu"
#define ORGANIZATION_NAME "GMRV - URJC"

#define APPLICATION_WIDTH 1366
#define APPLICATION_HEIGHT 768

#define USER_DATA_FOLDER "userdata/"

#define MAX_DATASET_NAME_LENGTH 10

#if defined(_WIN32) || defined(WIN32)
  #define SUPERUSER "powershell -Command \"Start-Process cmd -Verb -RunAs\""
#else
  #define SUPERUSER "gksudo"
#endif

#define STR_CSV "csv"
#define STR_JSON "json"
#define STR_SEG "seg"

#endif
