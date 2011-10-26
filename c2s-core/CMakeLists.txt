
cmake_minimum_required(VERSION 2.6)

add_library( c2s_core
    C2SRuntime
    C2SSocketListener
    C2SSocketAcceptHandler
    C2SGlobalSettings )

install( TARGETS 
	c2s_core
	DESTINATION lib )
	