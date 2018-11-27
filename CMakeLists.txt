cmake_minimum_required (VERSION 3.9.1)
cmake_minimum_required (VERSION 3.9.1 FATAL_ERROR)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
	message("_source_path_msvc ${_source_path_msvc}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

project (renderer CXX)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

assign_source_group(${SOURCE})

if(WIN32)
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")
endif(WIN32)

# Set path to find local modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${renderer_SOURCE_DIR}/cmake")


# Create main renderer target
add_executable(renderer ${SOURCE} ${HEADERS})

# Create test target
add_executable(test_model "src/test/test_model.cpp" "src/model.cpp" "src/mesh.cpp" "src/model.hpp")

#message (${CMAKE_CURRENT_LIST_DIR})

find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)

if(WIN32)
	set(CMAKE_PREFIX_PATH "libraries\\glew-2.1.0")
	set(CMAKE_LIBRARY_PATH "libraries\\glew-2.1.0\\lib\\Release\\Win32" "libraries\\SOIL2\\lib\\windows")
endif(WIN32)

find_package(GLEW REQUIRED)

include_directories("${SDL2_INCLUDE_DIR}")
include_directories("${GLEW_INCLUDE_DIR}")
# Include ASSIMP headers
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\assimp-4.1.0\\include"
		    "build\\libraries\\assimp-4.1.0\\include")
# Include SOIL2 headers
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\SOIL2\\src")

#include_directories("${PROJECT_SOURCE_DIR}\\src")

#include_directories("${SOIL2_INCLUDE_DIR}")

# Build ASSIMP
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory("libraries\\assimp-4.1.0")

find_library(SOIL2 SOIL2 "${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\SOIL2\\lib\\windows\\libsoil2.lib")

# Set targets links
target_link_libraries(renderer assimp ${SOIL2} ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(test_model assimp)

install(TARGETS renderer DESTINATION bin)

