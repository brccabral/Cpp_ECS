cmake_minimum_required(VERSION 4.4)
project(ECS)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_library(ECS INTERFACE)
add_library(ECS::ECS ALIAS ECS)
target_include_directories(ECS INTERFACE include)
target_link_libraries(ECS INTERFACE raylib)
target_compile_features(ECS INTERFACE cxx_std_20)

if (1)
    add_subdirectory(examples)
endif ()
