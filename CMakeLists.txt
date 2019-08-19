cmake_minimum_required (VERSION 3.15 FATAL_ERROR)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "cmake prefix"  FORCE)

include(cmake/list_all_properties.cmake)
project(gl00 LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			add_compile_options("/std:c++latest")
		endif ()
	endif ()

	# Force the console in Windows.
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console /NODEFAULTLIB:MSVCRT")
endif(WIN32)

# Set path to find local scripts.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(assimp CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB)

if (UNIX)
	find_package(GLEW)
	include_directories(${GLEW_INCLUDE_DIRS})
endif()

# Ensure glad is compiled.
set_source_files_properties(src/glad.c PROPERTIES LANGUAGE CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/apps)
#TODO: Add tests back in
#add_subdirectory(tests)

#[[
## gl00 Shared
add_library(gl00-shared
	SHARED
	${SOURCE}
	)

#set_target_properties(gl00-shared PROPERTIES OUTPUT_NAME gl00)

target_include_directories(gl00-shared PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(gl00-shared
	PUBLIC
	assimp::assimp
	OpenGL::GL
	)
#]]

## gl00 Static
add_library(gl00-static
	STATIC
	${SOURCE}
	)

set_target_properties(gl00-static PROPERTIES
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	DEBUG_POSTFIX d
	)

target_include_directories(gl00-static PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(gl00-static
	PUBLIC
	assimp::assimp
	OpenGL::GL
	ZLIB::ZLIB
	${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib/IrrXML$<$<CONFIG:Debug>:d>.lib
	)

#print_target_properties(assimp::assimp)
#print_target_properties(ZLIB::ZLIB)

#TODO: Add ability to dynamically link program; include the other runtimes.
#[[
install(FILES $<TARGET_FILE:SDL2::SDL2>
	CONFIGURATIONS Debug
	DESTINATION bin/Debug
	)
install(FILES $<TARGET_FILE:assimp::assimp>
	CONFIGURATIONS Debug
	DESTINATION bin/Debug
	)
#]]
install(TARGETS gl00-static
	LIBRARY DESTINATION lib/$<IF:$<CONFIG:Debug>,Debug,Release>
	RUNTIME DESTINATION bin/$<IF:$<CONFIG:Debug>,Debug,Release>
	ARCHIVE DESTINATION lib
	)
