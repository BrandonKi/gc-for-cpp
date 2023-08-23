cmake_minimum_required (VERSION 3.7.2)

set(project_name gc)
project(${project_name} VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(BIN_DIR ${EXECUTABLE_OUTPUT_PATH})

set(lib_name ${project_name}_lib)
add_library(${lib_name} OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(${project_name}
	$<TARGET_OBJECTS:${lib_name}>
)

# Tracy
#option( TRACY_ENABLE "" ON)
#option( TRACY_ON_DEMAND "" ON)
#option( TRACY_NO_EXIT "" ON)
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/lib//tracy)

target_include_directories(${lib_name} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include

#	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/tracy/public/tracy
)

target_link_libraries(${project_name} 
	# Tracy::TracyClient
)

target_compile_definitions(${lib_name}
	PRIVATE -DNOMINMAX
	PRIVATE -DVC_EXTRALEAN
	PRIVATE -DWIN32_LEAN_AND_MEAN

	# PRIVATE -DTRACY_ENABLE
)

install(TARGETS ${project_name} RUNTIME DESTINATION ${BIN_DIR}/)
set_target_properties(${project_name} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug/
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release/ )
