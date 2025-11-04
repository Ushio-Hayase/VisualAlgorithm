cmake_minimum_required(VERSION 3.30)
project(VisualAlgorithmCore)

set(CMAKE_CXX_STANDARD 20)

include_directories("include")

add_library(VisualAlgorithmCore STATIC src/file_read.cpp
        src/model.cpp)
