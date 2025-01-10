cmake_minimum_required(VERSION 3.29)
project(fight_night_finale_engine)

set(CMAKE_CXX_STANDARD 26)

include_directories(inc)

add_compile_options(/Z7)

add_executable(fnf_E src/main.cpp)
target_link_directories(fnf_E PUBLIC lib)
target_link_libraries(fnf_E PUBLIC aecore)
