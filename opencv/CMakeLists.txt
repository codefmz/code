cmake_minimum_required(VERSION 3.5.0)
project(opencvStudy VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

# 查找 OpenCV 包cmake会自动到这个目录去查找 xxconfig.cmake
set(OpenCV_DIR "D:/code_install_package/opencv/opencv/build/")
set(OpenCV_DLL_DIR "${OpenCV_DIR}/x64/vc15/bin")
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found at: ${OpenCV_DIR}")
    message(STATUS "OpenCv library: ${OpenCV_LIBS}")
    message(STATUS "OpenCv include: ${OpenCV_INCLUDE_DIRS}")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

add_subdirectory(src)