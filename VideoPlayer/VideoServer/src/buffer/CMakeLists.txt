file(GLOB SRC "./*.cpp")

add_library(buffer STATIC ${SRC})
target_include_directories(buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(buffer PUBLIC socket)