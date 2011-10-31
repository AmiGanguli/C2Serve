
cmake_minimum_required(VERSION 2.6)

add_executable( c2s_test_http_basic
    C2STestHttpBasic )

target_link_libraries( c2s_test_http_basic
  c2s_rest
  c2s_http
  c2s_core
  c2s_util
  ${Boost_LIBRARIES}
  ${C2S_SYSLIBS}
)

install( TARGETS 
	c2s_test_http_basic
	DESTINATION bin )

add_test( c2s_test_http_basic c2s_test_http_basic )
