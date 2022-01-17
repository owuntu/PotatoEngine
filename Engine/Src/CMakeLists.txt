cmake_minimum_required(VERSION 3.10)
project(PotatoEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(
    ${PARENT_DIR}/Extern/
    ${PARENT_DIR}/Extern/include
    ${PARENT_DIR}/Extern/glfw/include/GLFW
    ${PARENT_DIR}/Extern/glm
    )

set(SOURCES BVH.cpp
            BVHModelCreator.cpp
            Camera.cpp
            ClosestPointTest.cpp
            Game.cpp
            GLObject.cpp
            HelperDraw.cpp
            KdTree.cpp
            Mesh.cpp
            MeshModel.cpp
            MeshModelBVH.cpp
            Model.cpp
            ModelCreator.cpp
            PointCloudModel.cpp
            SinglePointModel.cpp
            )
add_library(PotatoEngine ${SOURCES})
