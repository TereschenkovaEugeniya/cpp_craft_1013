find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	unit_test_framework
	chrono
)

find_package( system_utilities ${SEARCH_PARAMETERS} COMPONENTS
	ts_queue
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries system_utilities Boost )
