
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework )

if ( Boost_FOUND )

  include_directories( ../ ${Boost_INCLUDE_DIRS} )
    
  if( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )

    add_executable( c2s_test_thread_posix C2SThreadTest )
    target_link_libraries( c2s_test_thread_posix ${Boost_LIBRARIES} pthread )
    install( TARGETS c2s_test_thread_posix DESTINATION bin )
    if ( NOT C2Serve_DISABLE_TESTING )
      add_test( c2s_test_thread_posix c2s_test_thread_posix )
    endif ( NOT C2Serve_DISABLE_TESTING )
    
  else( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    
    add_executable( c2s_test_thread_windows C2SThreadTest )
    target_link_libraries( c2s_test_thread_windows ${Boost_LIBRARIES} )
    install( TARGETS c2s_test_thread_windows DESTINATION bin )
    if ( NOT C2Serve_DISABLE_TESTING )
      add_test( c2s_test_thread_windows c2s_test_thread_windows )
    endif ( NOT C2Serve_DISABLE_TESTING )
        
  endif( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )

endif ( Boost_FOUND )