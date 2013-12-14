find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	thread
	system
	date_time
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)

