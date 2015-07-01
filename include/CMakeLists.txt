file(GLOB TILECONE_HEADERS "*.h*")

install(
  FILES ${TILECONE_HEADERS}
  DESTINATION include/tilecone
)
