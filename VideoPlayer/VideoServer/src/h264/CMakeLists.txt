file(GLOB SRC "./*.cpp")

add_library(h264 STATIC ${SRC})
target_link_libraries(h264 PUBLIC media session)
target_include_directories(h264 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})