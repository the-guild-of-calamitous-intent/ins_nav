# cmake_minimum_required(VERSION 3.10)
# project(phdMatrix VERSION 0.1.0 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# have a common find dependency command
# include("cmake/kevinBuild.cmake")

add_library(${PROJECT_NAME} SHARED
    cArray.cpp
    cBaseMath.cpp
    cMatrix.cpp
    cMLError.cpp
    cQuaternion.cpp
    cRand.cpp
    cRK4.cpp
    cVector.cpp
)
