cmake_minimum_required(VERSION 3.29)

project(fnf_E)


set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

add_library(fnf_E STATIC 
        ${SRC}
)

target_include_directories(fnf_E PUBLIC src)

target_include_directories(fnf_E PUBLIC inc)
target_link_directories(fnf_E PUBLIC lib)
target_link_libraries(fnf_E PUBLIC fmodL_vc)
target_link_libraries(fnf_E PUBLIC fmodstudioL_vc)
target_link_libraries(fnf_E PUBLIC aecore)
target_include_directories(fnf_E PUBLIC lib/json/single_include/nlohmann/)
# precompiled header
target_precompile_headers(fnf_E PRIVATE src/pch.hpp)

#copy res folder to build directory
add_custom_command(TARGET fnf_E POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

#copy FMOD dlls to build directory
add_custom_command(TARGET fnf_E POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/lib/fmodL.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/)

add_custom_command(TARGET fnf_E POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/lib/fmodstudioL.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/)
