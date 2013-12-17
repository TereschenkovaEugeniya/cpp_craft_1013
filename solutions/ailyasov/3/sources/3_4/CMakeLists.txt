find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	regex
	date_time
	filesystem
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries binaryio Boost )

