find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries reading_library writing_library Boost )
