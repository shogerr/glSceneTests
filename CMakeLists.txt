cmake_minimum_required (VERSION 3.9.1)
cmake_minimum_required (VERSION 3.9.1 FATAL_ERROR)

project (renderer CXX)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

if(WIN32)
	set("${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")
endif(WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${renderer_SOURCE_DIR}/cmake")

add_executable(renderer ${SOURCE} ${HEADERS})

add_executable(test_model "src/test/test_model.cpp" "src/model.cpp" "src/mesh.cpp" "src/model.hpp")

message (${CMAKE_CURRENT_LIST_DIR})

find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)

if(WIN32)
	set(CMAKE_PREFIX_PATH "libraries\\glew-2.1.0")
	set(CMAKE_LIBRARY_PATH "libraries\\glew-2.1.0\\lib\\Release\\Win32")
endif(WIN32)

find_package(GLEW REQUIRED)

include_directories("${SDL2_INCLUDE_DIR}")
include_directories("${GLEW_INCLUDE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\libraries\\assimp-4.1.0\\include"
		    "build\\libraries\\assimp-4.1.0\\include")
include_directories("libraries\\freeglut\\include")

set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

add_subdirectory("libraries\\assimp-4.1.0")


target_link_libraries(renderer assimp ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})
target_link_libraries(test_model assimp)

install(TARGETS renderer DESTINATION bin)

