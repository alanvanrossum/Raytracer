cmake_minimum_required(VERSION 3.2)
project(Raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    cube.mtl
    cube.obj
    dodgeColorTest.mtl
    dodgeColorTest.obj
    imageWriter.h
    main.cpp
    matrix.h
    mesh.cpp
    mesh.h
    raytracing.cpp
    raytracing.h
    traqueboule.h
    Vec3D.h
    Vertex.h)

add_executable(Raytracer ${SOURCE_FILES})