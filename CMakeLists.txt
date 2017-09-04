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
  ${CMAKE_SOURCE_DIR}/CMake/common )

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

# Allow warnings
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" )

include( Common )

common_find_package( Qt5 5.9 SYSTEM REQUIRED COMPONENTS Core Widgets Network )
common_find_package( manco REQUIRED )

common_find_package_post( )

list( APPEND VISHNU_DEPENDENT_LIBRARIES 
  Qt5::Core 
  Qt5::Gui 
  Qt5::Widgets 
  Qt5::Network
  manco 
)

add_subdirectory( vishnu )

if( MSVC )
  if( ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.6 )
    # Set Vishnu as startup project on Visual Studio
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
	  PROPERTY VS_STARTUP_PROJECT "Vishnu" )
  endif( )
endif( )

include( CPackConfig )
include( DoxygenRule )
