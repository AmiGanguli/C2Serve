
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework )

if ( Boost_FOUND )

include_directories( ../ ${Boost_INCLUDE_DIRS} )
    
if( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )

add_executable( threadtest_posix
    ThreadTest )

target_link_libraries( threadtest_posix ${Boost_LIBRARIES} pthread )

install( TARGETS threadtest_posix DESTINATION bin )

if ( NOT DISABLE_C2S_TESTING )
  add_test( threadtest_posix threadtest_posix )
endif ( NOT DISABLE_C2S_TESTING )


endif( NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    
add_subdirectory( boost )

endif ( Boost_FOUND )