include( utils )

if (minute_calculator_ROOT)
	set(minute_calculator_root ${minute_calculator_ROOT} )
elseif(NOT "$ENV{MINUTE_CALCULATOR_ROOT}" STREQUAL "")
	set(minute_calculator_root $ENV{MINUTE_CALCULATOR_ROOT} )
else()
	message(FATAL_ERROR "[ERROR]: No MINUTE_CALCULATOR_ROOT environment variable found")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
	if (!minute_calculator_FIND_QUIETLY)
		message(STATUS "CMAKE_BUILD_TYPE was not tuned, so we install there 'Debug'")
	endif()
else()
endif()

if (minute_calculator_DEBUG)
	message(STATUS "We going to look into '${minute_calculator_root}' (MINUTE_CALCULATOR_ROOT environment variable) folder.")
	create_string_from_list( component_list ${minute_calculator_FIND_COMPONENTS} )
	message(STATUS "We are looking for: '" ${component_list} "' components.")
endif(minute_calculator_DEBUG)

set(minute_calculator_FOUND FALSE)
set(minute_calculator_INCLUDE_DIRS "")
set(minute_calculator_LIBRARIES "")

if (UNIX)
	set(binary_search_folder ${minute_calculator_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
else(UNIX)
	set(binary_search_folder ${minute_calculator_root}/_build_${CMAKE_BUILD_TYPE}_${CMAKE_ADDRESS_MODEL}/bin_${CMAKE_ADDRESS_MODEL}/${CMAKE_BUILD_TYPE})
endif(UNIX)
if (NOT minute_calculator_FIND_QUIETLY)
	message(STATUS "Looking for libraries at " ${binary_search_folder})
endif()

if (minute_calculator_DEBUG)
	message(STATUS "Binary search folder: " ${binary_search_folder} )
endif(minute_calculator_DEBUG)

set(components minute_calculator)

foreach(component ${minute_calculator_FIND_COMPONENTS})

list_contains(we_should_find_${component} ${component} ${components})
if(we_should_find_${component})

	if (minute_calculator_DEBUG)
		message(STATUS "Searching for ${component} library. ")
	endif()
	set(minute_calculator_${component}_FOUND FALSE)
	set(minute_calculator_${component}_INCLUDE_DIRS NOTFOUND)
	set(minute_calculator_${component}_LIBRARIES NOTFOUND)

	find_path( minute_calculator_${component}_INCLUDE_DIR
			NAMES "CMakeLists.txt"
			HINTS "${minute_calculator_root}/sources/${component}/")
	set(minute_calculator_${component}_INCLUDE_DIRS ${minute_calculator_${component}_INCLUDE_DIR})

	find_library( minute_calculator_${component}_LIBRARY
		NAMES "${component}" 
		HINTS ${binary_search_folder} )
	set(minute_calculator_${component}_LIBRARIES ${minute_calculator_${component}_LIBRARY})

	if(NOT minute_calculator_FIND_QUIETLY)
		message(STATUS "minute_calculator ${component} component: ")
		create_string_from_list(headers ${minute_calculator_${component}_INCLUDE_DIRS})
		message(STATUS "   headers  :  " ${headers})
		create_string_from_list(libraries ${minute_calculator_${component}_LIBRARIES})
		message(STATUS "   libraries:  " ${libraries})
	endif()	

	if(NOT minute_calculator_${component}_INCLUDE_DIR OR NOT minute_calculator_${component}_LIBRARY)
		message(FATAL_ERROR " We can't find minute_calculator ${component} component")
	else()
		set(minute_calculator_INCLUDE_DIRS ${minute_calculator_INCLUDE_DIRS} ${minute_calculator_${component}_INCLUDE_DIRS})
		set(minute_calculator_LIBRARIES ${minute_calculator_LIBRARIES} ${minute_calculator_${component}_LIBRARIES})
	endif()
else()
	message(FATAL_ERROR "Unknown component: ${component}")
endif(we_should_find_${component})

endforeach( component )


if (minute_calculator_DEBUG)
	create_string_from_list(headers ${minute_calculator_INCLUDE_DIRS})
	message(STATUS "minute_calculator_INCLUDE_DIRS: " ${headers})
	create_string_from_list(libraries ${minute_calculator_LIBRARIES})
	message(STATUS "minute_calculator_LIBRARIES: " ${libraries})
endif(minute_calculator_DEBUG)

