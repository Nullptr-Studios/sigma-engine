cmake_minimum_required(VERSION 3.29)
project(engine_submodule LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(dev)
add_subdirectory(engine)
