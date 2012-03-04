
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/thread
)

add_library( c2s_log
  C2SLogBase
  C2SLogImplSimpleMessage
  C2SLogSimpleMessageFactory
 )

install( TARGETS 
	c2s_log
	DESTINATION lib
)

if ( NOT C2Serve_DISABLE_TESTING )
  add_subdirectory( test )
endif ( NOT C2Serve_DISABLE_TESTING )
