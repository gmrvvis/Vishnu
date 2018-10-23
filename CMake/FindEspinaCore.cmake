# Locate EspinaCore library and defines the following variables:
#
# EspinaCore_INCLUDE_DIR: EspinaCore include directories.
# EspinaCore_INCLUDE_DIRS: same as EspinaCore_INCLUDE_DIR.
# EspinaCore_LIBRARY: EspinaCore library name.
# EspinaCore_LIBRARIES: same as EspinaCore_LIBRARY.
# EspinaCore_FOUND: true if EspinaCore_LIBRARY and EspinaCore_INCLUDE_DIR have been found.

if( WIN32 )
  set( EspinaCore_INCLUDE_SEARCH_DIRS
    "$ENV{PROGRAMFILES}/EspinaCore/include"
  )
  set( EspinaCore_LIB_SEARCH_DIRS
    "$ENV{PROGRAMFILES}/EspinaCore/lib"
    "$ENV{PROGRAMFILES}/EspinaCore/bin"   
  )
else( )
  set( EspinaCore_INCLUDE_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
    "/opt/local/include"
  )
  set( EspinaCore_LIB_SEARCH_DIRS
    "/usr/lib64"
    "/usr/lib"
    "/usr/local/lib"
    "/opt/local/lib"
  )
endif( )

list( APPEND EspinaCore_INCLUDE_SEARCH_DIRS "${CMAKE_SOURCE_DIR}/include" )
list( APPEND EspinaCore_LIB_SEARCH_DIRS "${CMAKE_SOURCE_DIR}/lib" )

set( EspinaCore_ENV_INCLUDE_DIR $ENV{EspinaCore_INCLUDE_DIR} )
set( EspinaCore_ENV_LIBRARY $ENV{EspinaCore_LIBRARY} )
set( EspinaCore_ENV_INCLUDE_DIRS $ENV{EspinaCore_INCLUDE_DIRS} )
set( EspinaCore_ENV_LIBRARIES $ENV{EspinaCore_LIBRARIES} )

if( EspinaCore_ENV_INCLUDE_DIR )
  list( INSERT EspinaCore_INCLUDE_SEARCH_DIRS 0 "${EspinaCore_ENV_INCLUDE_DIR}" )
endif( )
if( EspinaCore_ENV_LIBRARY )
  list( INSERT EspinaCore_LIB_SEARCH_DIRS 0 "${EspinaCore_ENV_LIBRARY}" )
endif( )

if( EspinaCore_ENV_INCLUDE_DIRS )
  list( INSERT EspinaCore_INCLUDE_SEARCH_DIRS 0 "${EspinaCore_ENV_INCLUDE_DIRS}" )
endif( )
if( EspinaCore_ENV_LIBRARIES )
  list( INSERT EspinaCore_LIB_SEARCH_DIRS 0 "${EspinaCore_ENV_LIBRARIES}" )
endif( )

find_library( EspinaCore_LIBRARY 
  NAMES EspinaCore
  PATHS ${EspinaCore_LIB_SEARCH_DIRS}
)

find_path( EspinaCore_INCLUDE_DIR
  NAMES EspinaCore_Export.h
  PATHS ${EspinaCore_INCLUDE_SEARCH_DIRS}
)

set( EspinaCore_INCLUDE_DIRS ${EspinaCore_INCLUDE_DIR} )
set( EspinaCore_LIBRARIES ${EspinaCore_LIBRARY} )

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args(
  EspinaCore
  DEFAULT_MSG
  EspinaCore_LIBRARY 
  EspinaCore_INCLUDE_DIR
)

mark_as_advanced( EspinaCore_INCLUDE_DIR EspinaCore_LIBRARY )