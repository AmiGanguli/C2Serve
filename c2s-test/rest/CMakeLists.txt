
cmake_minimum_required(VERSION 2.6)

add_executable( c2s_test_rest
  C2STestRestMethodAdd
  C2STestRestMethodQueryFields
  C2STestRestMethodThreading
	C2STestRestResponse
	C2STestRestFixture
  C2STestRest )

target_link_libraries( c2s_test_rest c2s_rest c2s_httpclient c2s_http c2s_core c2s_util_io c2s_util ${Boost_LIBRARIES} ${C2S_SYSLIBS} )

install( TARGETS
	c2s_test_rest
	DESTINATION bin )

add_test( c2s_test_rest c2s_test_rest )
