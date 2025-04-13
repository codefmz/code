file(GLOB SRC "./*.cpp")

add_library(thread STATIC ${SRC})
set_target_properties(thread PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(thread PUBLIC sync log)
target_include_directories(thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})