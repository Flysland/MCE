cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(SRC_PATH src)
set(INCLUDE_PATH include)
set(
    SRC_FILES

    ${SRC_PATH}/world.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Werror -fPIC")

include_directories(${INCLUDE_PATH})

add_library(mce SHARED ${SRC_FILES})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g3")

add_subdirectory(tests)
