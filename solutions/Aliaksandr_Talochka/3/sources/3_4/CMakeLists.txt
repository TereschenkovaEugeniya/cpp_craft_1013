find_package( binary_reader ${SEARCH_PARAMETERS} COMPONENTS
	binary_reader
)

find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
)

message(STATUS " !!! ${BOOST_LIBRARYDIR} ")
compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost binary_reader )

