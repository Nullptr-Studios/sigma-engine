cmake_minimum_required(VERSION 3.29)
project(engine_test)

message("COÑO")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

add_executable(engine_test src/main.cpp
        ${SRC}
)

target_link_libraries(engine_test PRIVATE fnf_E)

message("debug mode")
target_link_libraries(fnf_E PUBLIC fmodL_vc)
target_link_libraries(fnf_E PUBLIC fmodstudioL_vc)

#copy res folder to build directory
add_custom_command(TARGET engine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

