file(GLOB SRC "./*.cpp")

add_library(sync STATIC ${SRC})
target_include_directories(sync PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})