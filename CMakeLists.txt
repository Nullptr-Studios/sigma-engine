cmake_minimum_required(VERSION 3.29)
project(fnf_E)

set(CMAKE_CXX_STANDARD 26)


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
        src/Camera.cpp
        src/Audio/AudioData.hpp
        src/Audio/AudioEngine.hpp
        src/Audio/AudioEngine.cpp
        src/Audio/AudioData.cpp
)
target_include_directories(fnf_E PUBLIC inc)
target_link_directories(fnf_E PUBLIC lib)
target_link_libraries(fnf_E PUBLIC fmodL_vc)
target_link_libraries(fnf_E PUBLIC fmodstudioL_vc)
target_link_libraries(fnf_E PUBLIC aecore)

#copy res folder to build directory
add_custom_command(TARGET fnf_E POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)