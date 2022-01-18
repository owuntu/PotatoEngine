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
    ${ENGINE_DIR}/Extern/glm
    )

set(SOURCES ClosestPointQuery.cpp
            main.cpp
            UnitTests.cpp
            )

if(BUILD_TEST)
    add_compile_definitions(UNIT_TESTS)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC ${ENGINE_DIR}
                                                 ${ENGINE_DIR}/Extern/dll
                                                )
target_link_libraries(${PROJECT_NAME} PotatoEngine
                                        libassimp.so)
