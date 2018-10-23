# QUAZIP_FOUND               - QuaZip library was found
# QUAZIP_INCLUDE_DIR         - Path to QuaZip include dir
# QUAZIP_INCLUDE_DIRS        - Path to QuaZip and zlib include dir (combined from QUAZIP_INCLUDE_DIR + ZLIB_INCLUDE_DIR)
# QUAZIP_LIBRARIES           - List of QuaZip libraries
# QUAZIP_ZLIB_INCLUDE_DIR    - The include dir of zlib headers

IF (QUAZIP_INCLUDE_DIRS AND QUAZIP_LIBRARIES)
	# in cache already
	SET(QUAZIP_FOUND TRUE)
ELSE (QUAZIP_INCLUDE_DIRS AND QUAZIP_LIBRARIES)
	# Qt5Core_FOUND not existing on Windows.
    #IF (QT5_FOUND)
	#    message( "QT5" )
    #    set(QUAZIP_LIB_VERSION_SUFFIX 5)
    #ELSE()
	#	message( "NO QT5" )
    #ENDIF()
	
	set(QUAZIP_LIB_VERSION_SUFFIX 5)
	
	IF (WIN32)
		FIND_PATH(QUAZIP_LIBRARY_DIR
			WIN32_DEBUG_POSTFIX d
            NAMES quazip${QUAZIP_LIB_VERSION_SUFFIX}.lib
			HINTS "C:/Programme/" "C:/Program Files"
			PATH_SUFFIXES QuaZip/lib
		)
		set(QUAZIP_LIBRARIES ${QUAZIP_LIBRARY_DIR}/quazip${QUAZIP_LIB_VERSION_SUFFIX}.lib)
		
		# FIND_LIBRARY not working on Windows.
		#message(${QUAZIP_LIBRARY_DIR})
		#message("quazip${QUAZIP_LIB_VERSION_SUFFIX}.lib")
        #FIND_LIBRARY(QUAZIP_LIBRARIES NAMES quazip${QUAZIP_LIB_VERSION_SUFFIX}.lib HINTS ${QUAZIP_LIBRARY_DIR})
		FIND_PATH(QUAZIP_INCLUDE_DIR NAMES quazip.h HINTS ${QUAZIP_LIBRARY_DIR}/../ PATH_SUFFIXES include/quazip${QUAZIP_LIB_VERSION_SUFFIX})
		FIND_PATH(QUAZIP_ZLIB_INCLUDE_DIR NAMES zlib.h HINTS ${QUAZIP_INCLUDE_DIR})
		#set(QUAZIP_ZLIB_INCLUDE_DIR "C:/libs/Qt/5.10.0/x64/msvc14/include/QtZlib/zlib.h")
	ELSE(WIN32)
		FIND_PACKAGE(PkgConfig)
#     pkg_check_modules(PC_QCA2 QUIET qca2)
		pkg_check_modules(PC_QUAZIP quazip)
		FIND_LIBRARY(QUAZIP_LIBRARIES
			WIN32_DEBUG_POSTFIX d
            NAMES quazip${QUAZIP_LIB_VERSION_SUFFIX}
			HINTS /usr/lib /usr/lib64
		)
		FIND_PATH(QUAZIP_INCLUDE_DIR quazip.h
			HINTS /usr/include /usr/local/include
			PATH_SUFFIXES quazip${QUAZIP_LIB_VERSION_SUFFIX}
		)
		FIND_PATH(QUAZIP_ZLIB_INCLUDE_DIR zlib.h HINTS /usr/include /usr/local/include)
	ENDIF (WIN32)
	INCLUDE(FindPackageHandleStandardArgs)
	SET(QUAZIP_INCLUDE_DIRS ${QUAZIP_INCLUDE_DIR} ${QUAZIP_ZLIB_INCLUDE_DIR})
	find_package_handle_standard_args(QUAZIP DEFAULT_MSG  QUAZIP_LIBRARIES QUAZIP_INCLUDE_DIR QUAZIP_ZLIB_INCLUDE_DIR QUAZIP_INCLUDE_DIRS)
ENDIF (QUAZIP_INCLUDE_DIRS AND QUAZIP_LIBRARIES)
