
cmake_minimum_required(VERSION 2.6)

find_package( Boost COMPONENTS unit_test_framework filesystem system )

if ( Boost_FOUND )

  include_directories(
    .
    ${Boost_INCLUDE_DIRS}
  )
  
  add_subdirectory( basic )  
  add_subdirectory( rest )

endif ( Boost_FOUND )