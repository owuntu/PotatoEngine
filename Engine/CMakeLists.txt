cmake_minimum_required(VERSION 3.10)
project(PotatoEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ./Extern/
    ./Extern/include
    ./Extern/glm
    )

set(SOURCE_DIR ./Src/)
set(SOURCES ${SOURCE_DIR}BVH.cpp
            ${SOURCE_DIR}BVHModelCreator.cpp
            ${SOURCE_DIR}ClosestPointTest.cpp
            ${SOURCE_DIR}Mesh.cpp
            ${SOURCE_DIR}MeshModel.cpp
            ${SOURCE_DIR}MeshModelBVH.cpp
            ${SOURCE_DIR}Model.cpp
            ${SOURCE_DIR}ModelCreator.cpp
            )
add_library(PotatoEngine ${SOURCES})
