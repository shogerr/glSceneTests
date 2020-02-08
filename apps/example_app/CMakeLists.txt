cmake_minimum_required(VERSION 3.15)

project(gl00-example)

#find_package(gl00 REQUIRED)

#find_package(glad REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    gl00::gl00-static
    glad
)