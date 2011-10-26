
cmake_minimum_required(VERSION 2.6)

set( GCC_CC_FLAGS "-g -Wall" )

add_definitions( -D_FILE_OFFSET_BITS=64 -DBOOST_ALL_NO_LIB )

if( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
  add_definitions( -DLINUX )
  set( CMAKE_CXX_FLAGS ${GCC_CC_FLAGS} )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set( CMAKE_CXX_FLAGS ${GCC_CC_FLAGS} )
  add_definitions( -DDARWIN )
  message( "configuration for mac osx" )
  set( OS_SUB_DIR "osx-gcc" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  message( "configuration for windows 32bit" )
  set( CMAKE_CXX_FLAGS "/EHa /W4" )
  add_definitions( -DWINXX )
  #add_definitions( -D_CRTDBG_MAP_ALLOC )
endif( CMAKE_SYSTEM_NAME STREQUAL "Linux")

message( "Building to " ${CMAKE_BINARY_DIR} )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions( -DDEBUG )
  if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    #for showing memory leaks
    #add_definitions( -D_CRTDBG_MAP_ALLOC ) 
  endif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions( -DRELEASE )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

enable_testing()

if( NOT DEFINED INSTALL_PREFIX )
	set( INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install )
endif( NOT DEFINED INSTALL_PREFIX )

set( CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} )
message( "Installing to \"" ${CMAKE_INSTALL_PREFIX} "\""  )
    
include_directories( 
  ${CORE_ROOT_DIR}/include )
  
## defines ######################################################################

set( HTDOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c2s-htdocs )
  
# system libs

if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
set( C2S_SYSLIBS boost_thread ws2_32 )
else ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
set( C2S_SYSLIBS pthread )
endif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )

set( Boost_USE_STATIC_LIBS ON )

include_directories( c2s-util )
include_directories( c2s-util/io )
include_directories( c2s-util/thread )

include_directories( c2s-core )
include_directories( c2s-http )
include_directories( c2s-rest )
include_directories( c2s-util )

add_subdirectory( c2s-core )
add_subdirectory( c2s-http )
add_subdirectory( c2s-rest )
add_subdirectory( c2s-util )

add_subdirectory( c2s-test )

## doxygen ######################################################################

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxygen.in ${CMAKE_CURRENT_BINARY_DIR}/Doxygen @ONLY )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxygen WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating API documentation" VERBATIM
)
endif(DOXYGEN_FOUND)
  
