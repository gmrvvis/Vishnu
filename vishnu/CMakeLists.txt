# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   Vishnu
#   2017-2018 (c) GMRV/URJC
#   Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
#   http://gmrv.es/gmrvvis/
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

source_group( \\ FILES CMakeLists.txt )

set( Name Vishnu )
set( NAME VISHNU )
set( namespace vishnu )
set( NAMESPACE VISHNU )
set( INCLUDE_NAME vishnu )
set( PROJECT_VERSION_ABI ${${PROJECT_NAME}_VERSION_ABI} )
configure_file( ${CMAKE_SOURCE_DIR}/CMake/common/cpp/api.h
  ${PROJECT_BINARY_DIR}/include/vishnu/api.h @ONLY )
configure_file( ${CMAKE_SOURCE_DIR}/CMake/common/cpp/version.h
  ${PROJECT_BINARY_DIR}/include/vishnu/version.h @ONLY )
configure_file( ${CMAKE_SOURCE_DIR}/CMake/common/cpp/version.cpp
  ${PROJECT_BINARY_DIR}/src/version.cpp @ONLY )
  
set( VISHNU_HEADERS
  ${PROJECT_BINARY_DIR}/include/vishnu/version.h
  resources.qrc
  Definitions.hpp
  MainWindow.h
  AppProcess.h  
  DataSetWindow.h
  RegExpInputDialog.h
  widgets/DataSetListWidget.h
  widgets/DataSetWidget.h
  widgets/UserDataSetListWidget.h
  widgets/UserDataSetWidget.h
  widgets/PropertiesTableWidget.h
  widgets/PropertiesWidget.h
  widgets/ZEQGroupListWidget.h
  widgets/ZEQGroupWidget.h
  widgets/PathsWidget.h
  model/Application.h
  model/AppsConfig.h
  model/UserPreferences.h
  model/UserDataSets.h
  model/UserDataSet.h
)

set( VISHNU_SOURCES
  ${PROJECT_BINARY_DIR}/src/version.cpp
  main.cpp
  MainWindow.cpp
  AppProcess.cpp
  DataSetWindow.cpp
  RegExpInputDialog.cpp
  widgets/DataSetListWidget.cpp
  widgets/DataSetWidget.cpp
  widgets/UserDataSetListWidget.cpp
  widgets/UserDataSetWidget.cpp
  widgets/PropertiesTableWidget.cpp
  widgets/PropertiesWidget.cpp
  widgets/ZEQGroupListWidget.cpp
  widgets/ZEQGroupWidget.cpp
  widgets/PathsWidget.cpp
  model/Application.cpp
  model/AppsConfig.cpp
  model/UserPreferences.cpp
  model/UserDataSets.cpp
  model/UserDataSet.cpp
)

set( VISHNU_LINK_LIBRARIES
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Network
  manco
  sp1common
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${PROJECT_BINARY_DIR}/include ${PROJECT_BINARY_DIR}/src )

if( MSVC )
  add_definitions( "-W4" )
  add_definitions( -DVISHNU_STATIC )
endif( )

common_application( vishnu )