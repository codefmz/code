cmake_minimum_required(VERSION 3.5.0)
project(linux_net VERSION 0.1.0)

set(CMAKE_C_FLAGS
    "-g"
    "-o"
    "-std=c11"
    "-Wunused"
    "-variable"
    "-pthread"
)

set(CMAKE_CXX_FLAGS
    "-g -pthread"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

enable_testing()

add_executable(${target} main.cpp)
target_link_libraries(${target} gtest lib${target})
target_link_libraries(
    ${target}
    -Wl,--whole-archive lib${target} -Wl,--no-whole-archive
) #避免静态库被优化掉
add_custom_command(TARGET ${target}
    COMMAND echo "compile ${target} test..."
)