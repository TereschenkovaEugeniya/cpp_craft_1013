find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	filesystem
	date_time
	chrono
	regex
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost )

