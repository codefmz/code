cmake_minimum_required(VERSION 3.5.0)
project(glfw3 VERSION 1.0.0)

set(CMAKE_CXX_FLAGS
    "-fno-inline -g -O0 -pthread"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/src/log
    # ${PROJECT_SOURCE_DIR}/src/poller
    # ${PROJECT_SOURCE_DIR}/src/sync
    # ${PROJECT_SOURCE_DIR}/src/event
    # ${PROJECT_SOURCE_DIR}/src/addr
    # ${PROJECT_SOURCE_DIR}/src/thread
    # ${PROJECT_SOURCE_DIR}/src/buffer
    # ${PROJECT_SOURCE_DIR}/src/server
    # ${PROJECT_SOURCE_DIR}/src/socket
    # ${PROJECT_SOURCE_DIR}/src/timer
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()