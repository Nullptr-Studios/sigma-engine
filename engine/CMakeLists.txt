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
        src/UI/UIImage.hpp
)

target_include_directories(fnf_E PUBLIC src)

target_include_directories(fnf_E PUBLIC inc)
target_link_directories(fnf_E PUBLIC lib)

target_link_libraries(fnf_E PUBLIC fmodL_vc)
target_link_libraries(fnf_E PUBLIC fmodstudioL_vc)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message("debug mode")
    
elseif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    message("release mode")
    target_link_libraries(fnf_E PUBLIC fmod_vc)
    target_link_libraries(fnf_E PUBLIC fmodstudio_vc)
    
else ()
    message("unknown mode")

endif ()

target_link_libraries(fnf_E PUBLIC aecore)
target_include_directories(fnf_E PUBLIC lib/json/single_include/nlohmann/)

# precompiled header
target_precompile_headers(fnf_E PRIVATE src/pch.hpp)
        
# glm library
add_subdirectory(lib/glm/glm)
target_link_libraries(fnf_E PUBLIC glm::glm)
target_include_directories(fnf_E PUBLIC lib/glm)
