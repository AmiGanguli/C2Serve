
cmake_minimum_required(VERSION 2.6)


#Set C2Serve_DISABLE_SYSTEM_SETTINGS to suppress incorporation of C2Serve system settings
#This may be useful if you want to integrate the C2Serve root CMakeLists.txt into your CMake build structure
if ( NOT C2Serve_DISABLE_SYSTEM_SETTINGS )
  cmake_policy( SET CMP0011 NEW )
  include( System.cmake )
  include( Doxygen.cmake )
endif ( NOT C2Serve_DISABLE_SYSTEM_SETTINGS )

## required system libs #####################################

if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( C2S_SYSLIBS ws2_32 )
else ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( C2S_SYSLIBS pthread )
endif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )

#############################################################

set( Boost_USE_STATIC_LIBS ON )
set( C2S_PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${C2S_PROJECT_ROOT_DIR} )
set( C2Serve_ROOT_DIR ${CMAKE_INSTALL_PREFIX} )
set( C2Serve_SKIP_CHECK_IF_LIBS_EXIST ON )

include( Install.cmake )

add_subdirectory( c2s-util )
add_subdirectory( c2s-log )
add_subdirectory( c2s-socket )
add_subdirectory( c2s-core )
add_subdirectory( c2s-http )
add_subdirectory( c2s-rest )
add_subdirectory( c2s-example )

if ( NOT C2Serve_DISABLE_TESTING )
  add_subdirectory( c2s-test )
endif ( NOT C2Serve_DISABLE_TESTING )
  


