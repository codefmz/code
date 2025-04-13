file(GLOB SRC "./*.cpp")

add_library(timer STATIC ${SRC})
set_target_properties(timer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(timer PUBLIC poller)
target_include_directories(timer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})