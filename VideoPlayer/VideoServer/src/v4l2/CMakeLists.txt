file(GLOB SRC "./*.cpp")

add_library(V4l2 STATIC ${SRC})
target_link_libraries(V4l2 PUBLIC media x264)
target_include_directories(V4l2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})