file(GLOB SRC "./*.cpp")

add_library(media STATIC ${SRC})
target_link_libraries(media PUBLIC sync thread)
target_include_directories(media PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})