cmake_minimum_required(VERSION 3.5.0)
project(DataStruct VERSION 0.1.0)

aux_source_directory(./src LOG_SRC)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/)
add_library(log STATIC ${LOG_SRC})
target_include_directories(log
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# link_directories(
#     ${PROJECT_SOURCE_DIR}/lib/Debug
# )

# add_executable(logExe main.cpp)
# target_link_libraries(logExe
#     liblog
# )
# target_include_directories(logExe
#     PRIVATE
#     ${PROJECT_SOURCE_DIR}/include
# )