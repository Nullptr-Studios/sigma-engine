cmake_minimum_required(VERSION 3.29)
project(fight_night_finale_engine)

set(CMAKE_CXX_STANDARD 26)



add_compile_options(/Z7)

#add_library(fnf_E STATIC src/main.cpp)
add_executable(fnf_E
        src/main.cpp
        src/GameManager.hpp
        src/GameManager.cpp
        src/Scene.hpp
        src/TestScene.cpp
        src/Object.hpp
        src/GameObject.hpp
        src/GameObject.cpp
        src/Camera.hpp
        src/Camera.cpp)
target_include_directories(fnf_E PUBLIC inc)
target_link_directories(fnf_E PUBLIC lib)
target_link_libraries(fnf_E PUBLIC aecore)
