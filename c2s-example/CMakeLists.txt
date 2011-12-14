
cmake_minimum_required(VERSION 2.6)

find_package( C2Serve )

if ( C2Serve_FOUND )

  include_directories(
    ${C2Serve_INCLUDE_DIRS}
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

endif ( C2Serve_FOUND )
	