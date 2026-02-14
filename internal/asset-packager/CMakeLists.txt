# Asset packer executable.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${PROJECT_SOURCE_DIR}/source/core/error.cpp)
add_executable(wr3ck-asset-packager ${SOURCE_FILES})

target_include_directories(
	wr3ck-asset-packager PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/source
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/vendor/zlib
	${PROJECT_SOURCE_DIR}/vendor/stb
)

# zlib
target_link_libraries(wr3ck-asset-packager PRIVATE zlibstatic)

# Package assets command.
set(PCK_NAME assets.pck)

set(ASSET_FILES "")
foreach(ASSET_ENTRY ${wr3ck-assets})
	if(IS_READABLE ${ASSET_ENTRY})
		if(IS_DIRECTORY ${ASSET_ENTRY})
			file(GLOB_RECURSE ASSET_FILES_GLOB CONFIGURE_DEPENDS ${ASSET_ENTRY}/**)
			list(APPEND ASSET_FILES ${ASSET_FILES_GLOB})
		else()
			list(APPEND ASSET_FILES ${ASSET_ENTRY})
		endif()
	endif()
endforeach()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asset_files.in "asset_files")

set(PCK_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${PCK_NAME})
add_custom_command(
	OUTPUT ${PCK_SOURCE}
	DEPENDS wr3ck-asset-packager ${ASSET_FILES} ${CMAKE_CURRENT_BINARY_DIR}/asset_files
	COMMAND wr3ck-asset-packager ${PCK_SOURCE} ${wr3ck-assets}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Packing assets"
)
add_custom_target(
	wr3ck-package-assets ALL
	DEPENDS ${PCK_SOURCE}
)

# Asset package to binary object (library) command
# NOTE: the architecture might not line up here with what the user might want,,, whooops
set(PCK_LIB ${PCK_SOURCE}.o)
add_custom_command(
	OUTPUT ${PCK_LIB}
	DEPENDS ${PCK_SOURCE}
	COMMAND ${CMAKE_OBJCOPY} ARGS
	-I binary
	-O elf64-x86-64
	-B i386:x86-64
	${PCK_NAME} ${PCK_NAME}.o
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Converting asset package into binary object"
)
add_custom_target(wr3ck-asset-packager-binary ALL
	DEPENDS ${PCK_LIB}
)

# Library!
set(wr3ck-assets-lib ${PCK_LIB} PARENT_SCOPE)

