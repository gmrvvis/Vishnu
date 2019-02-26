# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   Vishnu
#   2017-2019 (c) GMRV/URJC
#   Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
#   http://gmrv.es/gmrvvis/
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

include_directories(
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
  
set( VISHNU-CREATEAPPSCONFIG_SOURCES
  Vishnu-CreateAppsConfig.cpp
  ../vishnu/model/Application.cpp
  ../vishnu/model/AppsConfig.cpp
)

set( VISHNU-CREATEAPPSCONFIG_HEADERS
  ../vishnu/model/Application.h
  ../vishnu/model/AppsConfig.h
)

set( VISHNU-CREATEAPPSCONFIG_LINK_LIBRARIES
  Qt5::Core
  Qt5::Widgets
  VishnuCommon
)

common_application( Vishnu-CreateAppsConfig )

set( VISHNU-CREATEUSERPREFERENCES_SOURCES
  Vishnu-CreateUserPreferences.cpp
  ../vishnu/model/UserPreferences.cpp
)

set( VISHNU-CREATEUSERPREFERENCES_HEADERS
  ../vishnu/model/UserPreferences.h
)

set( VISHNU-CREATEUSERPREFERENCES_LINK_LIBRARIES
  Qt5::Core
  Qt5::Widgets
  VishnuCommon
)

common_application( Vishnu-CreateUserPreferences )
