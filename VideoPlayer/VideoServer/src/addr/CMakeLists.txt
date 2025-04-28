file(GLOB SRC "./*.cpp")

add_library(addr STATIC ${SRC})
target_include_directories(addr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})