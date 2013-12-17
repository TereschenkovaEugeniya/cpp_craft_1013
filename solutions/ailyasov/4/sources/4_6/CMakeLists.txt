find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	chrono
	date_time
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)

