
cmake_minimum_required(VERSION 2.6)

add_library( c2s_util 
    urlcode
    FileUtils
    StringUtils
    io/IOUtils
    io/Logger
    io/TimeLogger
    io/DateTimeLogger )

install( TARGETS 
  c2s_util
  DESTINATION lib )

add_subdirectory( thread )

