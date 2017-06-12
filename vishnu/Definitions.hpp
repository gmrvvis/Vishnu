#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define APPLICATION_NAME "Vishnu Launcher"

#ifdef defined(_WIN32) || defined(WIN32)
  #define SUPERUSER "powershell -Command \"Start-Process cmd -Verb -RunAs\""
#else
  #define SUPERUSER "gksudo"
#endif

#endif
