add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

include_directories (${Task_2_SOURCE_DIR}/lib)

link_directories(${Task_2_BINARY_DIR}/lib)

add_executable(small_buffer main.cpp)

target_link_libraries(small_buffer deal)