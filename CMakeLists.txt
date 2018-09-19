# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   Vishnu
#   2017-2018 (c) GMRV/URJC
#   Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
#   http://gmrv.es/gmrvvis/
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required( VERSION 3.1 )
set( PROJECT_VERSION_MAJOR "0" )
set( PROJECT_VERSION_MINOR "3" )
set( PROJECT_VERSION_PATCH "1" )
set( PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )
project( vishnu VERSION ${PROJECT_VERSION} )
set( vishnu_VERSION_ABI 5 )

// For EspINA.
add_definitions( -Wno-attributes )

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
set( VISHNU_MAINTAINER "Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>")
set( VISHNU_LICENSE "" )
set( COMMON_PROJECT_DOMAIN gmrv.es )

include( Common )

common_find_package( manco REQUIRED )
common_find_package( sp1common REQUIRED )

common_find_package_post( )

# No common.
find_package(Qt5 REQUIRED COMPONENTS Core Gui Network Widgets Test X11Extras)
find_package(QuaZip5 REQUIRED)
include_directories(${QUAZIP_INCLUDE_DIR})
find_package(VTK REQUIRED NO_MODULE)
include (${VTK_USE_FILE})
find_package(ITK REQUIRED)
include (${ITK_USE_FILE})
find_package( EspinaCore REQUIRED )
# Only needed for spine information.
# find_package( EspinaExtensions REQUIRED )

list( APPEND VISHNU_DEPENDENT_LIBRARIES 
  Qt5::Core 
  Qt5::Gui
  Qt5::Network 
  Qt5::Widgets 
  Qt5::Test
  Qt5::X11Extras
  manco
  sp1common
  ${QUAZIP_LIBRARIES}
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  ${EspinaCore_LIBRARIES}
)

# Only needed for spine information.
# ${EspinaExtensions_LIBRARIES}

add_subdirectory( vishnu )
add_subdirectory( examples )

if( MSVC )  
  # Set Vishnu as startup project on Visual Studio, CMake v3.6
  if( CMAKE_VERSION VERSION_GREATER 3.6 )
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vishnu )
  endif( )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")  
endif( )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )
include( DoxygenRule )

include( CPackConfig )
