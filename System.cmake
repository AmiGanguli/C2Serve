
cmake_minimum_required(VERSION 2.6)

set( GCC_CC_FLAGS "-g -Wall" )

add_definitions( -D_FILE_OFFSET_BITS=64 -DBOOST_ALL_NO_LIB )

if( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
  add_definitions( -DLINUX )
  set( CMAKE_CXX_FLAGS ${GCC_CC_FLAGS} )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set( CMAKE_CXX_FLAGS ${GCC_CC_FLAGS} )
  add_definitions( -DDARWIN )
  message( STATUS "Configuration for mac osx" )
  set( OS_SUB_DIR "osx-gcc" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  message( STATUS "Configuration for Windows 32bit" )
  set( CMAKE_CXX_FLAGS "/EHa /W4" )
  add_definitions( -DWINXX )
  #add_definitions( -D_CRTDBG_MAP_ALLOC )
endif( CMAKE_SYSTEM_NAME STREQUAL "Linux")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions( -DDEBUG )
  if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    #for showing memory leaks
    #add_definitions( -D_CRTDBG_MAP_ALLOC ) 
  endif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions( -DRELEASE )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
message( STATUS "Building to " ${CMAKE_BINARY_DIR} )

enable_testing()

if( NOT DEFINED INSTALL_PREFIX )
  set( INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install )
endif( NOT DEFINED INSTALL_PREFIX )

set( CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} )
message( STATUS "Installing to \"" ${CMAKE_INSTALL_PREFIX} "\""  )
