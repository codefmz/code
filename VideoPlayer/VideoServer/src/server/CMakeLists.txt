file(GLOB SRC "./*.cpp")

add_library(server STATIC ${SRC})
target_link_libraries(server PUBLIC poller event buffer)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})