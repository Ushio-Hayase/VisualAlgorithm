cmake_minimum_required(VERSION 3.30)
project(VisualAlgorithmCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(EXCLUDE_PATHS "cmake-build-debug" ".vscode")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.cpp")
foreach (TMP_PATH ${SRC})
    foreach (EXCLUDE_PATH ${EXCLUDE_PATHS})
        string(FIND ${TMP_PATH} ${EXCLUDE_PATH} RESULT)
        if (NOT ${RESULT} EQUAL -1)
            list(REMOVE_ITEM SRC ${TMP_PATH})
        endif ()
    endforeach (EXCLUDE_PATH)
endforeach (TMP_PATH)


add_library(VisualAlgorithmCore STATIC ${SRC})

target_include_directories(VisualAlgorithmCore PUBLIC ${CMAKE_SOURCE_DIR}/includes)