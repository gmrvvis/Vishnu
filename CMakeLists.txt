# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   Vishnu
#   2017-2019 (c) GMRV/URJC
#   Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
#   http://gmrv.es/gmrvvis/
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required( VERSION 3.1 )
set( PROJECT_VERSION_MAJOR "0" )
set( PROJECT_VERSION_MINOR "3" )
set( PROJECT_VERSION_PATCH "1" )
set( PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )
project( Vishnu VERSION ${PROJECT_VERSION} )
set( Vishnu_VERSION_ABI 5 )

set( USE_ESPINA OFF )

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
set( VISHNU_LICENSE "GPLv3" )
set( COMMON_PROJECT_DOMAIN gmrv.es )

include( Common )
common_find_package( Qt5Core SYSTEM REQUIRED )
common_find_package( Qt5Gui SYSTEM REQUIRED )
common_find_package( Qt5Widgets SYSTEM REQUIRED )
common_find_package( Qt5Network SYSTEM REQUIRED )
common_find_package( ManCo REQUIRED )
common_find_package( VishnuCommon REQUIRED )
common_find_package_post( )

list( APPEND VISHNU_DEPENDENT_LIBRARIES 
  Qt5::Core 
  Qt5::Gui
  Qt5::Network 
  Qt5::Widgets
  ManCo
  VishnuCommon
)

if( ${USE_ESPINA} )
  find_package(Boost REQUIRED COMPONENTS graph regex)
  set(Boost_USE_MULTITHREADED ON)
  link_directories(${Boost_LIBRARY_DIRS})
  include_directories(${Boost_INCLUDE_DIR})
  find_package(ITK REQUIRED)
  include (${ITK_USE_FILE})
  find_package(Qt5 REQUIRED COMPONENTS Test)
  if( UNIX )
    find_package(Qt5 REQUIRED COMPONENTS X11Extras)
  endif( )
  find_package(QuaZip5 REQUIRED)
  include_directories(${QUAZIP_INCLUDE_DIR})
  find_package(VTK REQUIRED NO_MODULE)
  include (${VTK_USE_FILE})

  # EspINA.
  find_package( EspinaCore REQUIRED )
  # Only needed for spine information.
  find_package( EspinaExtensions REQUIRED )

  list( APPEND VISHNU_DEPENDENT_LIBRARIES
    Qt5::Test
    ${QUAZIP_LIBRARIES}
    ${VTK_LIBRARIES}
    ${ITK_LIBRARIES}
    ${Boost_LIBRARIES}
    ${EspinaCore_LIBRARIES}
    # Only needed for spine information.
    ${EspinaExtensions_LIBRARIES}
  )  
  if( UNIX )
    list( APPEND VISHNU_DEPENDENT_LIBRARIES Qt5::X11Extras )
	add_definitions( -Wno-attributes )
	add_definitions( -fpermissive )
  endif( UNIX )  
endif( )

add_subdirectory( vishnu )
add_subdirectory( examples )

if( MSVC )  
  if( CMAKE_VERSION VERSION_GREATER 3.6 )
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Vishnu )
  endif( )
endif( )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )
include( DoxygenRule )

include( CPackConfig )
