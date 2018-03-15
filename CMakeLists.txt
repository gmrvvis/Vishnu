# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   Vishnu
#   2017 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required( VERSION 3.1 )
set( PROJECT_VERSION_MAJOR "1" )
set( PROJECT_VERSION_MINOR "0" )
set( PROJECT_VERSION_PATCH "0" )
set( PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )
project( vishnu VERSION ${PROJECT_VERSION} )
set( vishnu_VERSION_ABI 1 )

# Disable in source building
if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
  message( FATAL "no in source building allowed." )
endif( )

list( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common
)

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_definitions( -DDEBUG )
endif( )

include( GitExternal )

set( VISHNU_DESCRIPTION "Vishnu" )
set( COMMON_PROJECT_DOMAIN gmrv.es )

include( Common )

common_find_package( Qt5Core SYSTEM REQUIRED )
common_find_package( Qt5Widgets SYSTEM REQUIRED )
common_find_package( Qt5Network SYSTEM REQUIRED )
common_find_package( manco REQUIRED )
common_find_package( sp1common REQUIRED )

common_find_package_post( )

list( APPEND VISHNU_DEPENDENT_LIBRARIES 
  Qt5::Core 
  Qt5::Gui 
  Qt5::Widgets 
  Qt5::Network
  manco
  sp1common
)

add_subdirectory( vishnu )

if( MSVC )  
  # Set Vishnu as startup project on Visual Studio, CMake v3.6
  if( CMAKE_VERSION VERSION_GREATER 3.6 )
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vishnu )
  endif( )  
endif( )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )
include( DoxygenRule )

include( CPackConfig )
