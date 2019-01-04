cmake_minimum_required (VERSION 3.12.1)
cmake_minimum_required (VERSION 3.12.1 FATAL_ERROR)

#cmake_policy(SET CMP0074 NEW)

# Set project.
project(renderer LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "cmake prefix"  FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path to find local modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include useful functions.
include("cmake/assign_source_group.cmake")

# Add source and include directories, assign groups.
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
assign_source_group(${SOURCE})

add_subdirectory("${PROJECT_SOURCE_DIR}/src/test_app")
assign_source_group(${TEST_SOURCE})

add_subdirectory("${PROJECT_SOURCE_DIR}/include")
assign_source_group(${HEADERS})

# Force the console in Windows.
if(WIN32)
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")
endif(WIN32)

add_library(gl00-shared
	SHARED
	${SOURCE}
	${HEADERS}
)

add_library(gl00-static
	STATIC
	${SOURCE}
	${HEADERS}
)

set_target_properties(gl00-shared PROPERTIES OUTPUT_NAME gl00)
set_target_properties(gl00-static PROPERTIES OUTPUT_NAME gl00)

# Create main renderer target.
add_executable(renderer
	src/sdl/sdl_helper.hpp
	src/sdl/sdl_helper.cpp
	${TEST_SOURCE}
	${HEADERS}
)

# Ensure glad is compiled.
set_source_files_properties( src/glad.c PROPERTIES LANGUAGE CXX )

# Create test target.
add_executable(test_model "src/test/test_model.cpp" "src/model.cpp" "src/mesh.cpp" "include/gl00/model.hpp")
add_executable(test_scene_object "tests/test_scene_object.cpp" ${SOURCE})
add_executable(test_pointer "tests/test_pointer.cpp")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL2 REQUIRED)

include_directories(include)
include_directories("${SDL2_INCLUDE_DIR}")
include_directories("${assimp_INCLUDE_DIR}")
include_directories("${SOIL2_INCLUDE_DIR}")

# Set targets links
target_link_libraries(gl00-shared  ${SOIL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})
target_link_libraries(renderer ${assimp_LIBRARY} ${SDL2_LIBRARY} ${SOIL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})
target_link_libraries(test_model ${assimp_LIBRARY} ${OPENGL_gl_LIBRARY} ${SOIL2_LIBRARY} ${GLEW_LIBRARY})
target_link_libraries(test_scene_object ${assimp_LIBRARY} ${SDL2_LIBRARY} ${SOIL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY})

ASSIMP_COPY_BINARIES(${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS renderer gl00-shared gl00-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION bin)
