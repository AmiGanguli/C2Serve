
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-log
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/thread
)

set( C2S_SYSTEM_SPECIFIC_SOURCES C2SSocketClientConnectionUnix )
if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( C2S_SYSTEM_SPECIFIC_SOURCES C2SSocketClientConnectionWindows )
endif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )

add_library( c2s_socket
  C2SSocketListener
  C2SSocketAcceptHandler
  C2SSocketListenerThread
  C2SSocketClientConnection
  ${C2S_SYSTEM_SPECIFIC_SOURCES}
)

install( TARGETS 
	c2s_socket
	DESTINATION lib
)

add_subdirectory( test )
