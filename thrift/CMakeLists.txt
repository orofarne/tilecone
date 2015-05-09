set(thrift_file
	${CMAKE_CURRENT_SOURCE_DIR}/idx.thrift
)
set(thrift_files_cpp
	${CMAKE_CURRENT_BINARY_DIR}/idx_constants.cpp
	${CMAKE_CURRENT_BINARY_DIR}/idx_constants.h
	${CMAKE_CURRENT_BINARY_DIR}/idx_types.cpp
	${CMAKE_CURRENT_BINARY_DIR}/idx_types.h
)

add_custom_command(
	OUTPUT ${thrift_files_cpp}
	DEPENDS ${thrift_file}
	COMMAND ${THRIFT_COMPILER} --gen cpp --out ${CMAKE_CURRENT_BINARY_DIR} ${thrift_file}
)

add_custom_target(thrift_autogeneration_target ALL
	DEPENDS ${thrift_file}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(tilecone_thrift_lib STATIC
	${thrift_files_cpp}
)
add_dependencies(tilecone_thrift_lib thrift_autogeneration_target)
target_link_libraries(tilecone_thrift_lib
	${THRIFT_LIB}
)

set(TILECONE_THRIFT_LIB tilecone_thrift_lib PARENT_SCOPE)
set(TILECONE_THRIFT_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
