
find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
	regex
)

find_package( multicast_communication ${SEARCH_PARAMETERS} COMPONENTS
	multicast_communication
)

find_package( system_utilities ${SEARCH_PARAMETERS} COMPONENTS
	ts_queue
)


compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries minute_market_data minute_calculator multicast_communication system_utilities Boost )

