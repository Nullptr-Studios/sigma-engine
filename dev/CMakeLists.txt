cmake_minimum_required(VERSION 3.29)
project(engine_test)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

add_executable(engine_test src/main.cpp
        ${SRC}
)

target_link_libraries(engine_test PRIVATE sigma)
