find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	unit_test_framework
	filesystem
	date_time
	chrono
	regex
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost minute_calculator )
register_test( ${tests_name} 10 8 )

