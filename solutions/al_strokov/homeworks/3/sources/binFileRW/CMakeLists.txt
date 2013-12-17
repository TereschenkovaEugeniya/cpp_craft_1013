find_package( Boost 1.49 ${SEARCH_PARAMETERS} COMPONENTS 
    system 
    date_time
#    thread
)

compile_project( ${module_name} "*.cpp" "*.h" STATIC libraries Boost)
