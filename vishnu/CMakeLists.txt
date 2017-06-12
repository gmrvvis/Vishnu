source_group(\\ FILES CMakeLists.txt)

set(Name Vishnu)
set(NAME VISHNU)
set(namespace vishnu)
set(NAMESPACE VISHNU)
set(INCLUDE_NAME vishnu)
set(PROJECT_VERSION_ABI ${${PROJECT_NAME}_VERSION_ABI})
configure_file(${CMAKE_SOURCE_DIR}/CMake/common/cpp/api.h
  ${PROJECT_BINARY_DIR}/include/vishnu/api.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/CMake/common/cpp/version.h
  ${PROJECT_BINARY_DIR}/include/vishnu/version.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/CMake/common/cpp/version.cpp
  ${PROJECT_BINARY_DIR}/src/version.cpp @ONLY)
  
set(VISHNU_SOURCES
  ${PROJECT_BINARY_DIR}/src/version.cpp
  MainWindow.cpp
  main.cpp
  Application.cpp
  Process.cpp
  SyncGroup.cpp
  WidgetsGroup.cpp
)
set(VISHNU_HEADERS
  ${PROJECT_BINARY_DIR}/include/vishnu/version.h
  MainWindow.ui
  MainWindow.h
  Application.hpp
  Process.hpp
  SyncGroup.hpp
  resources.qrc
  WidgetsGroup.hpp
  Definitions.hpp
  utils/Auxiliars.hpp
)

set(VISHNU_LINK_LIBRARIES
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  manco
  ${CMAKE_THREAD_LIBS_INIT}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/include ${PROJECT_BINARY_DIR}/src)

MESSAGE( ${Boost_THREAD_LIBRARY} )

common_application(Vishnu)

