cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework )

if ( Boost_FOUND )

  include_directories(
    ${C2S_PROJECT_ROOT_DIR}/c2s-log
    ${Boost_INCLUDE_DIRS}
  )
  
  add_executable( c2s_test_log
      C2STestLogMain
      C2STestLogPublication
  )
  
  target_link_libraries( c2s_test_log
    c2s_log
    c2s_util
    ${Boost_LIBRARIES}
    ${C2S_SYSLIBS}
  )
  
  install( TARGETS 
  	c2s_test_log
  	DESTINATION bin )
  
  if ( NOT DISABLE_C2S_TESTING )
    add_test( c2s_test_log c2s_test_log )
  endif ( NOT DISABLE_C2S_TESTING )

endif ( Boost_FOUND )
  