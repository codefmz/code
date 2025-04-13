file(GLOB SRC "./*.cpp")

add_library(socket STATIC ${SRC})
set_target_properties(socket PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(socket PUBLIC log)
target_include_directories(socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})