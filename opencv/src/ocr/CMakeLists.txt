file(GLOB SRC "./*.cpp")

add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")

add_executable(ocr ${SRC})
target_link_libraries(ocr ${BASELIB})