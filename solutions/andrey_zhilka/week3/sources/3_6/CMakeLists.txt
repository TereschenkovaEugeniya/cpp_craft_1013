find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	filesystem
	date_time
	thread
)
compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost)

