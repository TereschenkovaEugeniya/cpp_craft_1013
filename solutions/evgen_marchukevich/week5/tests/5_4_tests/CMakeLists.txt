find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	date_time
	thread
 	chrono
 	regex
	system
	unit_test_framework
	filesystem
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost )
register_test( ${tests_name} 5 4 )

