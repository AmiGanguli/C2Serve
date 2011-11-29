
cmake_minimum_required(VERSION 2.6)

add_library( c2s_rest
    C2SRestPathSegment
    C2SRestPathParameter
    C2SRestPathSegmentList
    C2SRestMethodPrototype
    C2SRestResourcePrototype
    C2SRestResourceDescription
    C2SRestMatchMethodToRequest )

install( TARGETS 
	c2s_rest
	DESTINATION lib )
	