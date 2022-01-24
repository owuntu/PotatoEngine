cmake_minimum_required(VERSION 3.10)
project(PotatoEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ./Extern/
    ./Extern/include
    ./Extern/glfw/include/GLFW
    ./Extern/glm
    )

set(SOURCE_DIR ./Src/)
set(SOURCES ${SOURCE_DIR}BVH.cpp
            ${SOURCE_DIR}BVHModelCreator.cpp
            ${SOURCE_DIR}Camera.cpp
            ${SOURCE_DIR}ClosestPointTest.cpp
            ${SOURCE_DIR}Game.cpp
            ${SOURCE_DIR}GLObject.cpp
            ${SOURCE_DIR}HelperDraw.cpp
            ${SOURCE_DIR}KdTree.cpp
            ${SOURCE_DIR}Mesh.cpp
            ${SOURCE_DIR}MeshModel.cpp
            ${SOURCE_DIR}MeshModelBVH.cpp
            ${SOURCE_DIR}Model.cpp
            ${SOURCE_DIR}ModelCreator.cpp
            ${SOURCE_DIR}PointCloudModel.cpp
            ${SOURCE_DIR}PointCloudModelKdTree.cpp
            ${SOURCE_DIR}SinglePointModel.cpp
            ${SOURCE_DIR}ShaderObject/ShaderObject.cpp
            ${SOURCE_DIR}ShaderObject/ShaderProgram.cpp
            ./Extern/glad/glad.c
            )

add_library(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC ./Extern/lib/Linux
                                               ./Extern/dll)
target_link_libraries(${PROJECT_NAME} dl
                                      pthread
                                      glfw3
                                      libassimp.so)

