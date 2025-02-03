cmake_minimum_required(VERSION 3.15)

project(Controller2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions("-Wall -Wextra")

find_package(MAVSDK REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(controller ${SOURCES})

target_link_libraries(controller MAVSDK::mavsdk)
target_include_directories(controller PUBLIC "${PROJECT_SOURCE_DIR}/include")