file(GLOB SRC "./*.cpp")

add_library(log STATIC ${SRC})
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})