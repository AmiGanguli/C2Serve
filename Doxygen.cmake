
cmake_minimum_required(VERSION 2.6)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxygen.in ${CMAKE_CURRENT_BINARY_DIR}/Doxygen @ONLY )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxygen WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating API documentation" VERBATIM
)
endif(DOXYGEN_FOUND)
