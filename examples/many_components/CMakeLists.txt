cmake_minimum_required(VERSION 4.4)
project(ECS_many_components)

add_executable(many_components main.cpp)
target_link_libraries(many_components PRIVATE ECS)
