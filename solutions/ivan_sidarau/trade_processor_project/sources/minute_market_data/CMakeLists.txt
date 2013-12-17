
find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
	regex
	filesystem
)

find_package( system_utilities ${SEARCH_PARAMETERS} COMPONENTS
	ts_queue
	logger
	file_logger
	queue_logger
	task_processor
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries minute_calculator multicast_communication common system_utilities Boost )

