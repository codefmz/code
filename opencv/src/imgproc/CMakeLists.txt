file(GLOB SRC "./*.cpp")

add_definitions(-DRES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")

add_executable(imgproc ${SRC})
target_link_libraries(imgproc gtest ${OpenCV_LIBS})
add_custom_command(TARGET imgproc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OpenCV_DLL_DIR}/opencv_world3411d.dll"
        "$<TARGET_FILE_DIR:imgproc>"
)