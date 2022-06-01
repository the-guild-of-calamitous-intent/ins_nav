cmake_minimum_required(VERSION 3.10)
project(phdMatrix VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# have a common find dependency command
# include("cmake/kevinBuild.cmake")

# add_library(${PROJECT_NAME} SHARED matrix.cpp hello.cppm)

add_subdirectory(src)
