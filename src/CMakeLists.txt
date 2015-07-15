set(tilecone_SRC
	bucket.cpp
	zigzag.cpp
	index.cpp
	db.cpp
	db_c.cpp
)

add_library(tilecone SHARED
	${tilecone_SRC}
)

target_link_libraries(tilecone
	${Boost_LIBRARIES}
)

install(
	TARGETS tilecone
	LIBRARY DESTINATION lib
)

add_library(tilecone_mt SHARED
	${tilecone_SRC}
)

set_target_properties(tilecone_mt PROPERTIES
  COMPILE_FLAGS "-DTILECONE_MT"
)

target_link_libraries(tilecone_mt
	${Boost_LIBRARIES}
)

install(
	TARGETS tilecone_mt
	LIBRARY DESTINATION lib
)
