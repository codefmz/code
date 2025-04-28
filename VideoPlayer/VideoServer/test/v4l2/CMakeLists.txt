add_executable(v4l2Test v4l2Test.cpp)
target_link_libraries(v4l2Test h264 V4l2 gtest scheduler)
add_custom_command(TARGET v4l2Test
    COMMAND echo "compile v4l2Test test..."
)