add_executable(h264Test h264Test.cpp)
target_link_libraries(h264Test h264 gtest scheduler)
add_custom_command(TARGET h264Test
    COMMAND echo "compile h264Test test..."
)