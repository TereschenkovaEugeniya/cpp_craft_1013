include_directories(../)
find_package( binary_reader ${SEARCH_PARAMETERS} COMPONENTS
	binary_reader
)

find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
#	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost binary_reader)

