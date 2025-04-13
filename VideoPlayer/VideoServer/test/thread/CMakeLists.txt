add_executable(threadTest threadTest.cpp)
target_link_libraries(threadTest gtest thread)
add_custom_command(TARGET threadTest
    COMMAND echo "compile socketTest test..."
)