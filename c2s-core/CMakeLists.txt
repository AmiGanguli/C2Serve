
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-log
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/thread
)

add_library( c2s_core
  C2SRuntime
  C2SSettings
  C2SSocketListener
  C2SSocketAcceptHandler
)

install( TARGETS 
	c2s_core
	DESTINATION lib
)

add_subdirectory( test )
