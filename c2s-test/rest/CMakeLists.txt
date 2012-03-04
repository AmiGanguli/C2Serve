
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest
  ${C2S_PROJECT_ROOT_DIR}/c2s-http
  ${C2S_PROJECT_ROOT_DIR}/c2s-core
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/thread
)

add_executable( c2s_test_rest
  ../C2STestServerThread
  ../C2STestServerRunner
  C2STestRestMethodAdd
  C2STestRestMethodAddDuplicate
  C2STestRestMethodQueryFields
  C2STestRestMethodThreading
  C2STestRestMethodMediaTypeConverter
  C2STestRestMethodInvalidEntity
  C2STestRestEntityUnstreamerXML
  C2STestRestEntityUnstreamerJSON
  C2STestRestRequest
	C2STestRestResponse
	C2STestRestFixture
	C2STestRestCheckServerResponses
	C2STestRestCheckMultiThreading
	C2STestRestServerInitialization
  C2STestRest )

target_link_libraries( c2s_test_rest
  c2s_rest
  c2s_httpclient
  c2s_http
  c2s_core
  c2s_log
  c2s_util
  ${Boost_LIBRARIES}
  ${C2S_SYSLIBS}
)

install( TARGETS
	c2s_test_rest
	DESTINATION bin )

if ( NOT DISABLE_C2S_TESTING )
  add_test( c2s_test_rest c2s_test_rest )
endif ( NOT DISABLE_C2S_TESTING )
