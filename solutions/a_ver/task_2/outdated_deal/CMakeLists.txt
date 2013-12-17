add_definitions( -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

include_directories (${Task_2_SOURCE_DIR}/lib)

link_directories(${Task_2_BINARY_DIR}/lib)

add_executable(outdated_deal main.cpp)
add_executable(outdated_deal_sort main_sort.cpp)
#add_executable(outdated_deal_filesort main_filesort.cpp)

target_link_libraries(outdated_deal deal)
target_link_libraries(outdated_deal_sort deal)
#target_link_libraries(outdated_deal_filesort deal)
