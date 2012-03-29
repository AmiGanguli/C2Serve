
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework )

if ( Boost_FOUND )

	include_directories(
	  ${C2S_PROJECT_ROOT_DIR}/c2s-socket
	  ${Boost_INCLUDE_DIRS}
	)
	
	add_executable( c2s_test_socket
    C2STestCore
    C2STestSocketClient
    C2STestSocketListener
    C2STestSocketListenerDataPull
    C2STestSocketListenerDataHandling
	)
	
	target_link_libraries( c2s_test_socket
	  c2s_socket
	  c2s_log
	  c2s_util
	  ${Boost_LIBRARIES}
	  ${C2S_SYSLIBS}
	)
	
	install( TARGETS 
		c2s_test_socket
		DESTINATION bin )
	
	if ( NOT C2Serve_DISABLE_TESTING )
	  add_test( c2s_test_socket c2s_test_socket )
	endif ( NOT C2Serve_DISABLE_TESTING )
	
endif ( Boost_FOUND )