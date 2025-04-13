add_executable(socketTest socketTest.cpp)
target_link_libraries(socketTest gtest socket addr server)
add_custom_command(TARGET socketTest
    COMMAND echo "compile socketTest test..."
)