file(GLOB TILECONE_BENCH_SOURCES "*.cpp")

add_executable(tilecone_bench ${TILECONE_BENCH_SOURCES})

add_dependencies(tilecone_bench
  celero
  tilecone
)

target_link_libraries(tilecone_bench
  ${CELERO_LIBRARY}
  ${Boost_LIBRARIES}
  tilecone
  "dl"
)

run_bench(tilecone_bench)

add_executable(tilecone_bench_mt ${TILECONE_BENCH_SOURCES})

add_dependencies(tilecone_bench_mt
  celero
  tilecone_mt
)

target_link_libraries(tilecone_bench_mt
  ${CELERO_LIBRARY}
  ${Boost_LIBRARIES}
  tilecone_mt
  "dl"
)

run_bench(tilecone_bench_mt)
