cmake_minimum_required(VERSION 2.7)

if(UNIX)

    set( CMAKE_CXX_FLAGS  "-std=c++98 -pedantic -pedantic-errors -W -Wall -Wextra -Wconversion" )
    #set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

endif(UNIX)

add_subdirectory(find_islands)
add_subdirectory(rational_numbers)
add_subdirectory(reverse_string)

