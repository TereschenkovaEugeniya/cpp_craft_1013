find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
)

add_definitions( -DBOOST_FILESYSTEM_VERSION=3 )

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)

