include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB TILECONE_TEST_SOURCES "*.cpp")

add_executable(tilecone_test ${TILECONE_TEST_SOURCES})

add_dependencies(tilecone_test
	gtest
	tilecone
)

target_link_libraries(tilecone_test
	tilecone
	${GTEST_BOTH_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	"dl"
)

run_test(tilecone_test)

add_executable(tilecone_test_mt ${TILECONE_TEST_SOURCES})

add_dependencies(tilecone_test_mt
	gtest
	tilecone_mt
)

target_link_libraries(tilecone_test_mt
	tilecone_mt
	${GTEST_BOTH_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	"dl"
)

run_test(tilecone_test_mt)
