file(GLOB SRC "./*.cpp")

add_library(buffer STATIC ${SRC})
set_target_properties(buffer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(buffer PUBLIC socket)