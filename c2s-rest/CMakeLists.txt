
cmake_minimum_required(VERSION 2.6)

add_library( c2s_rest
    C2SRestResource
    C2SRestMethodPrototype
    C2SRestPathSegmentList
    C2SRestResourceDescription )

install( TARGETS 
	c2s_rest
	DESTINATION lib )
	