
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework )

if ( Boost_FOUND )

	include_directories(
	  ${C2S_PROJECT_ROOT_DIR}/c2s-core
	  ${Boost_INCLUDE_DIRS}
	)
	
	add_executable( c2s_test_core
    C2STestCore
    C2STestSocketClient
    C2STestSocketListener
    C2STestSocketListenerDataPull
    C2STestSocketListenerDataHandling
	)
	
	target_link_libraries( c2s_test_core
	  c2s_core
	  c2s_log
	  c2s_util
	  ${Boost_LIBRARIES}
	  ${C2S_SYSLIBS}
	)
	
	install( TARGETS 
		c2s_test_core
		DESTINATION bin )
	
	if ( NOT DISABLE_C2S_TESTING )
	  add_test( c2s_test_core c2s_test_core )
	endif ( NOT DISABLE_C2S_TESTING )
	
endif ( Boost_FOUND )