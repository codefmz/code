file(GLOB SRC "./*.cpp")

add_library(session STATIC ${SRC})
target_link_libraries(session PUBLIC rtp rtsp)
target_include_directories(session PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})