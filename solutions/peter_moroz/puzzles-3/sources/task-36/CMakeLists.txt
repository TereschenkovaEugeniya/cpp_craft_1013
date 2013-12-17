find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	date_time
	system
	thread
)

if(NOT Boost_FOUND)
  message(STATUS "Failed to find Boost")
  return()
else()
  include_directories(${Boost_INCLUDE_DIRS})
endif()

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries common Boost )