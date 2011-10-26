
cmake_minimum_required(VERSION 2.6)

add_library( c2s_util_io 
    IOUtils
    Logger
    TimeLogger
    DateTimeLogger )

install( TARGETS 
	c2s_util_io
	DESTINATION lib )
	