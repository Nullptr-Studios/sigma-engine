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

# FMOD shit
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message("debug mode")
    target_link_libraries(fnf_E PUBLIC fmodL_vc)
    target_link_libraries(fnf_E PUBLIC fmodstudioL_vc)
    
elseif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    message("release mode")
    target_link_libraries(fnf_E PUBLIC fmod_vc)
    target_link_libraries(fnf_E PUBLIC fmodstudio_vc)
    
else ()
    message("unknown mode")

endif ()

#copy res folder to build directory
add_custom_command(TARGET engine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

#copy FMOD dlls to build directory
add_custom_command(TARGET engine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/lib/fmodL.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/)

add_custom_command(TARGET engine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/lib/fmodstudioL.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/)
