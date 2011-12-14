#
#  Script to find necessary C2Serve includes and libraries
#  This script sets the following variables:
#    C2Serve_INCLUDE_DIRS - List of include directories
#    C2Serve_LIBRARIES    - List of libraries
#    C2Serve_FOUND        - True if all necessary C2Serve components have been found
#

if ( NOT DEFINED C2S_ROOT_DIR )
  #TODO: Search all possible install paths
  set( C2S_ROOT_DIR /usr/local )
endif( NOT DEFINED C2S_ROOT_DIR )

message( STATUS "C2Serve root directory: " ${C2S_ROOT_DIR} )

set( C2Serve_FOUND TRUE )

set( C2S_INCLUDE_DIRS_RELATIVE
  c2s-util
  c2s-core
  c2s-http
  c2s-rest
)

foreach( C2S_CURRENT_INCLUDE_DIR_RELATIVE ${C2S_INCLUDE_DIRS_RELATIVE} )
  set( C2S_CURRENT_INCLUDE_DIR_ABSOLUTE ${C2S_ROOT_DIR}/include/c2s/${C2S_CURRENT_INCLUDE_DIR_RELATIVE} )
  if ( NOT EXISTS ${C2S_CURRENT_INCLUDE_DIR_ABSOLUTE} )
    set( C2Serve_FOUND FALSE )
    set( C2S_MISSING_INCLUDES ${C2S_CURRENT_INCLUDE_DIR_ABSOLUTE} )
  else ( NOT EXISTS ${C2S_CURRENT_INCLUDE_DIR_ABSOLUTE} )
    set( C2Serve_INCLUDE_DIRS ${C2Serve_INCLUDE_DIRS} ${C2S_CURRENT_INCLUDE_DIR_ABSOLUTE} )
  endif ( NOT EXISTS ${C2S_CURRENT_INCLUDE_DIR_ABSOLUTE} )
endforeach( C2S_CURRENT_INCLUDE_DIR_RELATIVE ${C2S_INCLUDE_DIRS_RELATIVE} )

if ( NOT C2Serve_FOUND )
  message( STATUS "Could NOT find C2Serve" )
  if ( C2S_MISSING_INCLUDES )
    message( STATUS " Missing include paths:" )
    foreach( C2S_MISSING_INCLUDE ${C2S_MISSING_INCLUDES} )
      message( STATUS "  " ${C2S_MISSING_INCLUDE} )  
    endforeach( C2S_MISSING_INCLUDE ${C2S_MISSING_INCLUDES} )
  endif( C2S_MISSING_INCLUDES )
endif ( NOT C2Serve_FOUND )