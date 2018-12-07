cmake_minimum_required (VERSION 3.9.1)
cmake_minimum_required (VERSION 3.9.1 FATAL_ERROR)

# Set project.
project(renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path to find local modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${renderer_SOURCE_DIR}/cmake")

# Include usefule functions.
include("cmake/assign_source_group.cmake")

# Add source and include directories, assign groups.
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
assign_source_group(${SOURCE})

add_subdirectory("${PROJECT_SOURCE_DIR}/src/test_app")
assign_source_group(${TEST_SOURCE})

add_subdirectory("${PROJECT_SOURCE_DIR}/include")
assign_source_group(${HEADERS})

# Force the console.
if(WIN32)
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")
endif(WIN32)

add_library(gl00 SHARED ${SOURCE} ${HEADERS})

# Create main renderer target
add_executable(renderer ${TEST_SOURCE} ${HEADERS})

# Create test target
add_executable(test_model "src/test/test_model.cpp" "src/model.cpp" "src/mesh.cpp" "include/model.hpp")
add_executable(test_scene_object "test/test_scene_object.cpp" ${SOURCE})

#message (${CMAKE_CURRENT_LIST_DIR})

find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)

if(WIN32)
	set(CMAKE_PREFIX_PATH "libraries\\glew-2.1.0")
	set(CMAKE_LIBRARY_PATH "libraries\\glew-2.1.0\\lib\\Release\\Win32" "libraries\\SOIL2\\lib\\windows")
endif(WIN32)

find_package(GLEW REQUIRED)

include_directories("include")
include_directories("${SDL2_INCLUDE_DIR}")
include_directories("${GLEW_INCLUDE_DIR}")
# Include ASSIMP headers
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\assimp-4.1.0\\include"
		    "build\\libraries\\assimp-4.1.0\\include")
# Include SOIL2 headers
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\SOIL2\\src")

# Build ASSIMP
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory("libraries\\assimp-4.1.0")

find_library(SOIL2 SOIL2 "${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\SOIL2\\lib\\windows\\libsoil2.lib")

# Set targets links
target_link_libraries(gl00 assimp ${SOIL2} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(renderer assimp ${SDL2_LIBRARY} ${SOIL2} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(test_model assimp ${OPENGL_LIBRARIES} ${SOIL2} ${GLEW_LIBRARY})
target_link_libraries(test_scene_object assimp ${SDL2_LIBRARY} ${SOIL2} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} )

install(TARGETS renderer gl00 DESTINATION bin)

