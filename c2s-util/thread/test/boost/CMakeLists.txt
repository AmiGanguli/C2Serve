
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS thread )

add_definitions( -DUSE_BOOST_THREADS )
    
add_executable( threadtest_boost
    ../ThreadTest )
    
if( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    
target_link_libraries( threadtest_boost ${Boost_LIBRARIES} pthread )
   
else( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    
target_link_libraries( threadtest_boost boost_unit_test_framework boost_thread )
   
endif( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    
install( TARGETS threadtest_boost DESTINATION bin )

if ( NOT DISABLE_C2S_TESTING )
  add_test( threadtest_boost threadtest_boost )
endif ( NOT DISABLE_C2S_TESTING )
