# Locate EspinaExtensions library and defines the following variables:
#
# EspinaExtensions_INCLUDE_DIR: EspinaExtensions include directories.
# EspinaExtensions_INCLUDE_DIRS: same as EspinaExtensions_INCLUDE_DIR.
# EspinaExtensions_LIBRARY: EspinaExtensions library name.
# EspinaExtensions_LIBRARIES: same as EspinaExtensions_LIBRARY.
# EspinaExtensions_FOUND: true if EspinaExtensions_LIBRARY and EspinaExtensions_INCLUDE_DIR have been found.

if( WIN32 )
  set( EspinaExtensions_INCLUDE_SEARCH_DIRS
    "$ENV{PROGRAMFILES}/EspinaExtensions/include"
  )
  set( EspinaExtensions_LIB_SEARCH_DIRS
    "$ENV{PROGRAMFILES}/EspinaExtensions/lib"
    "$ENV{PROGRAMFILES}/EspinaExtensions/bin"   
  )
else( )
  set( EspinaExtensions_INCLUDE_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
    "/opt/local/include"
  )
  set( EspinaExtensions_LIB_SEARCH_DIRS
    "/usr/lib64"
    "/usr/lib"
    "/usr/local/lib"
    "/opt/local/lib"
  )
endif( )

list( APPEND EspinaExtensions_INCLUDE_SEARCH_DIRS "${CMAKE_SOURCE_DIR}/include" )
list( APPEND EspinaExtensions_LIB_SEARCH_DIRS "${CMAKE_SOURCE_DIR}/lib" )

set( EspinaExtensions_ENV_INCLUDE_DIR $ENV{EspinaExtensions_INCLUDE_DIR} )
set( EspinaExtensions_ENV_LIBRARY $ENV{EspinaExtensions_LIBRARY} )
set( EspinaExtensions_ENV_INCLUDE_DIRS $ENV{EspinaExtensions_INCLUDE_DIRS} )
set( EspinaExtensions_ENV_LIBRARIES $ENV{EspinaExtensions_LIBRARIES} )

if( EspinaExtensions_ENV_INCLUDE_DIR )
  list( INSERT EspinaExtensions_INCLUDE_SEARCH_DIRS 0 "${EspinaExtensions_ENV_INCLUDE_DIR}" )
endif( )
if( EspinaExtensions_ENV_LIBRARY )
  list( INSERT EspinaExtensions_LIB_SEARCH_DIRS 0 "${EspinaExtensions_ENV_LIBRARY}" )
endif( )

if( EspinaExtensions_ENV_INCLUDE_DIRS )
  list( INSERT EspinaExtensions_INCLUDE_SEARCH_DIRS 0 "${EspinaExtensions_ENV_INCLUDE_DIRS}" )
endif( )
if( EspinaExtensions_ENV_LIBRARIES )
  list( INSERT EspinaExtensions_LIB_SEARCH_DIRS 0 "${EspinaExtensions_ENV_LIBRARIES}" )
endif( )

find_library( EspinaExtensions_LIBRARY 
  NAMES EspinaExtensions
  PATHS ${EspinaExtensions_LIB_SEARCH_DIRS}
)

find_path( EspinaExtensions_INCLUDE_DIR
  NAMES EspinaExtensions/LibraryExtensionFactory.h
  PATHS ${EspinaExtensions_INCLUDE_SEARCH_DIRS}
)

find_path( EspinaExtensions_Export_INCLUDE_DIR
  NAMES EspinaExtensions_Export.h
  PATHS ${EspinaExtensions_INCLUDE_SEARCH_DIRS}
)
list( APPEND EspinaExtensions_INCLUDE_DIR ${EspinaExtensions_Export_INCLUDE_DIR} )

set( EspinaExtensions_INCLUDE_DIRS ${EspinaExtensions_INCLUDE_DIR} )
set( EspinaExtensions_LIBRARIES ${EspinaExtensions_LIBRARY} )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args(
  EspinaExtensions
  DEFAULT_MSG
  EspinaExtensions_LIBRARY 
  EspinaExtensions_INCLUDE_DIR
)

mark_as_advanced( EspinaExtensions_INCLUDE_DIR EspinaExtensions_LIBRARY )