add_library(tilecone SHARED
	bucket.cpp
	zigzag.cpp
	index.cpp
	db.cpp
	db_c.cpp
)

target_link_libraries(tilecone
	${Boost_LIBRARIES}
)

install(
	TARGETS tilecone
	LIBRARY DESTINATION lib
)
