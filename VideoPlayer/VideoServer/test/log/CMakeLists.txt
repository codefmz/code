add_executable(logTest logTest.cpp)
target_link_libraries(logTest gtest
    -Wl,--whole-archive log -Wl,--no-whole-archive
)
add_custom_command(TARGET logTest
    COMMAND echo "compile logTest test..."
)