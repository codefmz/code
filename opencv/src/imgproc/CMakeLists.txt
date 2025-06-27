file(GLOB SRC "./*.cpp")

add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")

add_executable(imgproc ${SRC})
target_link_libraries(imgproc ${BASELIB})