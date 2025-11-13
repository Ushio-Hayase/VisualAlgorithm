cmake_minimum_required(VERSION 3.30)
project(VisualAlgorithmCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB VISUAL_ALGORITHM_CORE_SOURCE_FILES CONFIGURE_DEPENDS "./src/*.cpp")

add_library(VisualAlgorithmCore STATIC ${VISUAL_ALGORITHM_CORE_SOURCE_FILES})

target_include_directories(VisualAlgorithmCore PUBLIC ${CMAKE_SOURCE_DIR}/includes)