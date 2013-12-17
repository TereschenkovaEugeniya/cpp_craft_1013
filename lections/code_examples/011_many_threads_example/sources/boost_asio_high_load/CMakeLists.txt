find_package( Boost 1.54 ${SEARCH_PARAMETERS} COMPONENTS
	filesystem
	thread
	regex
	date_time
	system
	unit_test_framework
	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost)
