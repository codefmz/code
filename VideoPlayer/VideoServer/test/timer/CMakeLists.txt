add_executable(timerTest timerTest.cpp)
target_link_libraries(timerTest gtest timer)
add_custom_command(TARGET timerTest
    COMMAND echo "compile timerTest test..."
)