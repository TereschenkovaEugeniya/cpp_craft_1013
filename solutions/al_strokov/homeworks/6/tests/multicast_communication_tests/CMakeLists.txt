find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	unit_test_framework
	filesystem
	thread
)

find_library( multicast_communication  ${SEARCH_PARAMETERS}
    PATHS SOURCE_DIR sources/multicast_communication/) 

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost multicast_communication )
register_test( ${tests_name} multicast_communication_tests 1)
