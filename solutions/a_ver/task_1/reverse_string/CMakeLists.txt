add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

add_executable(reverse_string main.cpp)
add_executable(reverse_string_kmp main_kmp.cpp)