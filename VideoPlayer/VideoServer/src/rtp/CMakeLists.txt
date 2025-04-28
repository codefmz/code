file(GLOB SRC "./*.cpp")

add_library(rtp STATIC ${SRC})
target_link_libraries(rtp PUBLIC addr socket timer scheduler media)
target_include_directories(rtp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})