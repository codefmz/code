file(GLOB SRC "./*.cpp")

add_library(rtsp STATIC ${SRC})
target_link_libraries(rtsp PUBLIC media server session)
target_include_directories(rtsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})