file(GLOB TILECONE_BENCH_SOURCES "*.cpp")
add_executable(tilecone_bench ${TILECONE_BENCH_SOURCES})

add_dependencies(tilecone_bench
  celero
)

target_link_libraries(tilecone_bench
  ${CELERO_LIBRARY}
  ${Boost_LIBRARIES}
  tilecone
  "dl"
)

run_bench(tilecone_bench)
