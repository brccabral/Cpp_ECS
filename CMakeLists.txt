cmake_minimum_required(VERSION 4.4)
project(ECS)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)
add_executable(ECS main.cpp)
target_link_libraries(ECS PUBLIC raylib)
