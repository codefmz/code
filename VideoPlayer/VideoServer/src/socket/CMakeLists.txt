file(GLOB SRC "./*.cpp")

add_library(socket STATIC ${SRC})
target_link_libraries(socket PUBLIC log addr)
target_include_directories(socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})