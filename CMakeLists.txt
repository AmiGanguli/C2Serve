
cmake_minimum_required(VERSION 2.6)

cmake_policy( SET CMP0011 NEW )
include( ${CMAKE_SOURCE_DIR}/System.cmake )
include( ${CMAKE_SOURCE_DIR}/Doxygen.cmake )
  
## required system libs #####################################

if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( C2S_SYSLIBS boost_thread ws2_32 )
else ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( C2S_SYSLIBS pthread )
endif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )

#############################################################

set( Boost_USE_STATIC_LIBS ON )

include_directories(
  c2s-util
  c2s-util/io
  c2s-util/thread
  c2s-core
  c2s-http
  c2s-rest
  c2s-util
)

add_subdirectory( c2s-core )
add_subdirectory( c2s-http )
add_subdirectory( c2s-rest )
add_subdirectory( c2s-util )
add_subdirectory( c2s-test )

