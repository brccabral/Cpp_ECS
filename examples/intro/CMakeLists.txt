cmake_minimum_required(VERSION 4.4)
project(ECS_intro)

add_executable(intro main.cpp)
target_link_libraries(intro PRIVATE ECS)
