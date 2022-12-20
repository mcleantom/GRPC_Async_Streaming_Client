
find_package(Threads)
set(SOURCES client.h client.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(
    AsyncStreamingClient ${SOURCES}
)
target_link_libraries(
    AsyncStreamingClient
        PRIVATE
            ProtoObjects
)
