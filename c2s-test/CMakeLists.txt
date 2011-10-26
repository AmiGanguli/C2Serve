
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework filesystem system )

if ( Boost_FOUND )

include_directories( ${Boost_INCLUDE_DIRS} )

add_executable( c2s_test_http_basic
    C2STestHttpBasic )

#add_executable( c2s_test_http_run
#    C2STestHttpRun )

add_executable( c2s_test_rest
	C2STestRestResponse
    C2STestRest )

target_link_libraries( c2s_test_http_basic c2s_rest c2s_http c2s_core c2s_util_io c2s_util ${Boost_LIBRARIES} ${C2S_SYSLIBS} )
#target_link_libraries( c2s_test_http_run c2s_rest c2s_httpclient c2s_http c2s_core c2s_util_io c2s_util boost_unit_test_framework boost_filesystem boost_system ${C2S_SYSLIBS} )
target_link_libraries( c2s_test_rest c2s_rest c2s_httpclient c2s_http c2s_core c2s_util_io c2s_util ${Boost_LIBRARIES} ${C2S_SYSLIBS} )

install( TARGETS 
	c2s_test_http_basic
#	c2s_test_http_run
	c2s_test_rest
	DESTINATION bin )

add_test( c2s_test_http_basic c2s_test_http_basic )
#add_test( c2s_test_http_run c2s_test_http_run )
add_test( c2s_test_rest c2s_test_rest )

endif ( Boost_FOUND )