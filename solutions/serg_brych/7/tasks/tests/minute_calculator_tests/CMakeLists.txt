find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	unit_test_framework
	filesystem
	date_time
	chrono
	regex
)


find_package( system_utilities ${SEARCH_PARAMETERS} COMPONENTS
	ts_queue
)

find_package( multicast_communication ${SEARCH_PARAMETERS} COMPONENTS
	multicast_communication
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost minute_calculator system_utilities multicast_communication )
register_test( ${tests_name} 10 8 )

