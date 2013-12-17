find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	date_time
	system
	thread
 	filesystem
	date_time
 	chrono
 	regex
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)

