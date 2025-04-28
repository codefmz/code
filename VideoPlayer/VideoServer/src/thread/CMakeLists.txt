file(GLOB SRC "./*.cpp")

add_library(thread STATIC ${SRC})
target_link_libraries(thread PUBLIC sync log)
target_include_directories(thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})