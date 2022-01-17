cmake_minimum_required(VERSION 3.10)
project(ClosestPointQuery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

get_filename_component(ENGINE_DIR ../Engine ABSOLUTE)
include_directories(
    ${ENGINE_DIR}
    ${ENGINE_DIR}/Src
    ${ENGINE_DIR}/Extern
    ${ENGINE_DIR}/Extern/include
    ${ENGINE_DIR}/Extern/glfw/include/GLFW
    ${ENGINE_DIR}/Extern/glm
    )

set(SOURCES ClosestPointQuery.cpp
            main.cpp
            UnitTests.cpp
            )

add_executable(ClosestPointQuery ${SOURCES})

target_link_directories(ClosestPointQuery PUBLIC ${ENGINE_DIR})
target_link_libraries(ClosestPointQuery libPotatoEngine.a)
