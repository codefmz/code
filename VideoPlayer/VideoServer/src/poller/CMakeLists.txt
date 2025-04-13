file(GLOB SRC "./*.cpp")

add_library(poller STATIC ${SRC})
set_target_properties(poller PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(poller PUBLIC log event)
target_include_directories(poller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})