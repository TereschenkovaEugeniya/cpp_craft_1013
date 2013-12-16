find_package( minute_calculator ${SEARCH_PARAMETERS} COMPONENTS
	minute_calculator
)

find_package( multicast_communication ${SEARCH_PARAMETERS} COMPONENTS
	multicast_communication
)


find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
	regex
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries multicast_communication minute_calculator Boost )

