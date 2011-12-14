
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest
  ${C2S_PROJECT_ROOT_DIR}/c2s-http
  ${C2S_PROJECT_ROOT_DIR}/c2s-core
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/io
)

add_executable( c2s_example
	C2SExampleMain
	C2SExampleRestMethodPrototypeSayHello
)

target_link_libraries( c2s_example
  c2s_rest
  c2s_http
  c2s_core
  c2s_util
  ${C2S_SYSLIBS}
)

install( TARGETS 
	c2s_example
	DESTINATION bin
)
	