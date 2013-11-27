find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	date_time
	system
	thread
 	filesystem
	unit_test_framework
	chrono
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost )
register_test( ${tests_name} 5 4 )

