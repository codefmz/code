file(GLOB SRC "./*.cpp")

add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")

add_executable(helloCv ${SRC})
target_link_libraries(helloCv gtest ${OpenCV_LIBS})
add_custom_command(TARGET helloCv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DLL_DIR}/opencv_world3411d.dll"
        "$<TARGET_FILE_DIR:helloCv>"
)