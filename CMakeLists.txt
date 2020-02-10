cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

project(gl00 LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "cmake prefix" FORCE)

# Set path to find local modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

option(STATIC_LINK "Statically link executable." ON)
option(BUILD_EXAMPLES "Build the library examples." TRUE)
option(BUILD_TESTS "Build the tests." TRUE)

if(VCPKG_TARGET_TRIPLET MATCHES x64-windows-static)
	set(STATIC_LINK ON)
endif()

# Create compile commands with Build.Ninja.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GenerateExportHeader)

# Import some debugging functions.
include(cmake/list_all_properties.cmake)
include(cmake/list_all_variables.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

if(UNIX)
	find_package(assimp REQUIRED)
	find_package(glm REQUIRED)
	find_package(GLEW)
	include_directories(${GLEW_INCLUDE_DIRS})
else()
	find_package(assimp CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)
endif()

find_package(OpenGL REQUIRED)
find_package(ZLIB)

# Ensure glad source files are compiled.
#set_source_files_properties(src/glad.c PROPERTIES LANGUAGE CXX)

# Add Glad
add_subdirectory(${PROJECT_SOURCE_DIR}/glad)

# Variable SOURCE for source files.
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

# Variable HEADERS for headers.
add_subdirectory(${PROJECT_SOURCE_DIR}/include)

##
## Examples & Applications
##
if (${BUILD_EXAMPLES})
	if(UNIX)
		find_package(SDL2 REQUIRED)
	else()
		find_package(SDL2 CONFIG REQUIRED)
	endif()

	add_subdirectory(${PROJECT_SOURCE_DIR}/apps/test_app)
	add_subdirectory(${PROJECT_SOURCE_DIR}/apps/example_app)
endif()

##
## Tests
##
if (${BUILD_TESTS})
	add_subdirectory(${PROJECT_SOURCE_DIR}/test/unit)
endif()

##
## GL00 Static Library
##
if(${STATIC_LINK})
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
		#OUTPUT_NAME $<IF:$<BOOL:${STATIC_LINK}>,gl00-static,gl00>
		OUTPUT_NAME gl00
		#TODO Should be -static with gcc
		#TODO Using the same code for static and shared?
		$<<BOOL:${MSVC}>:MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${STATIC_LINK}>>:DLL>>
		)
	target_link_libraries(gl00-static
		PRIVATE
			$<$<BOOL:${MSVC}>:-subsystem:console>
			glad
		PUBLIC
			OpenGL::GL
			$<IF:$<BOOL:${UNIX}>,${assimp_LIBRARY},assimp::assimp>
			$<$<BOOL:${STATIC_LINK}>:ZLIB::ZLIB>
			$<$<BOOL:${STATIC_LINK}>:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib/IrrXML$<$<CONFIG:Debug>:d>.lib>
		)

	install(TARGETS gl00-static
		EXPORT gl00Targets
		RUNTIME DESTINATION lib
		)
else()

##
## GL00 Runtime Library
##

# TODO To build dynamic libraries, elements will need to be marked as exported.
# NOTE Export symbols (generate_export_header) for a shared library in windows.

	add_library(gl00
		SHARED
		${SOURCE}
		)
	add_library(gl00::gl00 ALIAS gl00)
	set_target_properties(gl00 PROPERTIES
		DEBUG_POSTFIX d
		)
	target_include_directories(gl00
		PUBLIC ${gl00_SOURCE_DIR}/include
		)
	target_link_libraries(gl00
		PRIVATE
			"$<$<BOOL:${MSVC}>:-subsystem:console>"
			glad
		PUBLIC
			$<IF:$<BOOL:${UNIX}>,${assimp_LIBRARY},assimp::assimp>
			$<$<BOOL:${UNIX}>:GLEW::GLEW>
			OpenGL::GL
		)
	# Install all of the runtime components.
	if(WIN32)
		install(
			FILES
				$<TARGET_FILE:SDL2::SDL2>
				$<TARGET_FILE:assimp::assimp>
				$<$<CONFIG:Debug>:$<TARGET_FILE_DIR:gl00>/zlibd1.dll>
			DESTINATION
				bin/$<CONFIG>
			)
	endif()

	generate_export_header(gl00
		BASE_NAME gl00
		EXPORT_MACRO_NAME gl00_EXPORT
	)

	install(TARGETS gl00
		EXPORT gl00Targets
		RUNTIME DESTINATION bin/$<CONFIG>
		)
endif()

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
