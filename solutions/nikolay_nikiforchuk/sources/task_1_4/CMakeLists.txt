cmake_minimum_required(VERSION 2.8)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/output.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
	
add_executable(task_1_4 main.cpp)