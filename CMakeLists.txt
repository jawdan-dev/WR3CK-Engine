cmake_minimum_required(VERSION 3.29)
project(wr3ck-engine)

# Main executable.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)
target_link_options(${PROJECT_NAME} PRIVATE -static -s)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 23
)

# Submodule validate function.
function(check_and_add_subdirectory dir)
	if(NOT EXISTS ${dir}/CMakeLists.txt)
		message(FATAL_ERROR "Fatal: Submodule " ${dir} " not found\n")
	endif()

	add_subdirectory(${dir})
endfunction()

# GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# GLEW.
set(glew-cmake_BUILD_SHARED FALSE)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)
check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)

# zlib.
check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/zlib)
target_link_libraries(${PROJECT_NAME} PRIVATE zlibstatic)

# Asset Packager.
if(DEFINED wr3ck-assets)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/internal/asset-packager)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${wr3ck-assets-lib})
endif()
