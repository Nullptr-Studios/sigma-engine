cmake_minimum_required(VERSION 3.29)
project(sigma)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

add_library(sigma STATIC 
        ${SRC}
)

target_include_directories(sigma PUBLIC src)

target_include_directories(sigma PUBLIC inc)
target_link_directories(sigma PUBLIC lib)

target_link_libraries(sigma PUBLIC fmodL_vc)
target_link_libraries(sigma PUBLIC fmodstudioL_vc)

target_link_libraries(sigma PUBLIC aecore)
target_include_directories(sigma PUBLIC lib/json/single_include/nlohmann/)

# glm library
add_subdirectory(lib/glm)
target_include_directories(sigma PUBLIC lib/glm)
target_link_libraries(sigma PUBLIC glm::glm)

# precompiled header
target_precompile_headers(sigma PRIVATE src/pch.hpp)
        


