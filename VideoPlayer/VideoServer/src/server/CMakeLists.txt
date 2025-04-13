file(GLOB SRC "./*.cpp")

add_library(server STATIC ${SRC})
set_target_properties(server PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(server PUBLIC poller event buffer)