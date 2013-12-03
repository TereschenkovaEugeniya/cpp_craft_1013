find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	unit_test_framework
	filesystem
	date_time
	chrono
	regex
)

find_package( multicast_communication ${SEARCH_PARAMETERS} COMPONENTS
	multicast_communication
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost multicast_communication minute_calculator minute_market_data )
register_test( ${tests_name} 60 40 )

