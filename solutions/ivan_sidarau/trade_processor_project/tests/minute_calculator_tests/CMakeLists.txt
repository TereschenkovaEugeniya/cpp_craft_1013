find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	unit_test_framework
	filesystem
	date_time
	regex
	thread
	chrono
)

find_package( system_utilities ${SEARCH_PARAMETERS} COMPONENTS
	ts_queue
	logger
	file_logger
	queue_logger
	task_processor
)


compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} multicast_communication common system_utilities Boost )
register_test( ${tests_name} 5 4 )

