cmake_minimum_required (VERSION 3.15 FATAL_ERROR)

project(gl00 LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "cmake prefix" FORCE)

# Set path to find local modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(STATIC_LINK "Statically link executable." OFF)

if(VCPKG_TARGET_TRIPLET MATCHES x64-windows-static)
	set(STATIC_LINK ON)
endif()

# Import some debugging functions.
include(cmake/list_all_properties.cmake)
include(cmake/list_all_variables.cmake)

# Macro for setting cpp standard in MSVC.
include(cmake/msvc_cpp_latest.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	# Use /std:c++latest
	msvc_use_cpp_lastest()
	# Force the console in Windows.
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console /NODEFAULTLIB:MSVCRT")
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

if(UNIX)
	find_package(SDL2 REQUIRED)
	find_package(assimp REQUIRED)
	find_package(glm REQUIRED)
	find_package(GLEW)
	include_directories(${GLEW_INCLUDE_DIRS})
else()
	find_package(SDL2 CONFIG REQUIRED)
	find_package(assimp CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)
endif()

find_package(OpenGL REQUIRED)
find_package(ZLIB)

# Ensure glad source files are compiled.
#set_source_files_properties(src/glad.c PROPERTIES LANGUAGE CXX)

# Add Glad
add_subdirectory(${PROJECT_SOURCE_DIR}/glad)

# Add Source Files
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

#add_subdirectory(${PROJECT_SOURCE_DIR}/include)

# Add Applications
add_subdirectory(${PROJECT_SOURCE_DIR}/apps)

#TODO: Add tests back in
#add_subdirectory(tests)

##
## GL00 Static Library
##
add_library(gl00-static
	STATIC
	${SOURCE}
	)
add_library(gl00::gl00-static ALIAS gl00-static)
target_include_directories(gl00-static
	PUBLIC
	${gl00_SOURCE_DIR}/include
	)
set_target_properties(gl00-static PROPERTIES
	DEBUG_POSTFIX d
	#TODO Should be -static with gcc
	MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${STATIC_LINK}>>:DLL>
	OUTPUT_NAME $<IF:$<BOOL:${STATIC_LINK}>,gl00-static,gl00>
	)
target_link_libraries(gl00-static
	PRIVATE
		glad
	PUBLIC
		OpenGL::GL
		$<IF:$<BOOL:${UNIX}>,${assimp_LIBRARY},assimp::assimp>
		$<$<BOOL:${STATIC_LINK}>:ZLIB::ZLIB>
		$<$<BOOL:${STATIC_LINK}>:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib/IrrXML$<$<CONFIG:Debug>:d>.lib>
	)

##
## GL00 Runtime Library
##
if(NOT ${STATIC_LINK})
	add_library(gl00
		SHARED
		${SOURCE}
		)
	add_library(gl00::gl00 ALIAS gl00)
	set_target_properties(gl00 PROPERTIES
		DEBUG_POSTFIX d
		)
	target_include_directories(gl00
		PRIVATE ${gl00_SOURCE_DIR}/include
		)
	target_link_libraries(gl00
		PRIVATE
			glad
		PUBLIC
			$<IF:$<BOOL:${UNIX}>,${assimp_LIBRARY},assimp::assimp>
			$<$<BOOL:${UNIX}>:GLEW::GLEW>
			OpenGL::GL
		)

	# Install all of the runtime components.
	install(FILES $<TARGET_FILE:SDL2::SDL2>
		DESTINATION bin/$<CONFIG>
		)
	install(FILES $<TARGET_FILE:assimp::assimp>
		DESTINATION bin/$<CONFIG>
		)
	install(FILES $<TARGET_FILE_DIR:gl00>/zlib$<$<CONFIG:Debug>:d>1.dll
		DESTINATION bin/$<CONFIG>
		)

	install(TARGETS gl00
		EXPORT gl00Targets
		RUNTIME DESTINATION bin/$<CONFIG>
		)
endif()

# Install the static libraries.
install(TARGETS gl00-static
	EXPORT gl00Targets
	ARCHIVE DESTINATION lib
	)

install(EXPORT gl00Targets
	FILE gl00Targets.cmake
	NAMESPACE gl00::
	DESTINATION lib/cmake/gl00
	)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	INSTALL_DESTINATION lib/cmake/gl00
	)

export(EXPORT gl00Targets
	FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
	NAMESPACE gl00::
	)
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE gl00)

# Debug Environment
#print_target_properties(assimp::assimp)
#print_target_properties(OpenGL::OpenGL)
#print_target_properties(gl00)

#dump_cmake_variables()
