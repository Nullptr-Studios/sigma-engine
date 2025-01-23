project(test)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

add_executable(engine_test src/main.cpp
        ${SRC}
)

target_link_libraries(engine_test fnf_E)
