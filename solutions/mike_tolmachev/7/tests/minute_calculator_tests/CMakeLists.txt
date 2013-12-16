find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	unit_test_framework
	filesystem
	date_time
	chrono
	regex
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost multicast_communication )
register_test( ${tests_name} 130 130 )

