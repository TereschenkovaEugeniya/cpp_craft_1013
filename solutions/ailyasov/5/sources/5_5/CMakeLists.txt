find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	date_time
	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)

