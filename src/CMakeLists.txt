add_library(tilecone SHARED
	tile.cpp
	index.cpp
)

add_dependencies(tilecone
	${TILECONE_THRIFT_LIB}
)


target_link_libraries(tilecone
	${TILECONE_THRIFT_LIB}
)
