file(GLOB SRC "./*.cpp")

add_library(event STATIC ${SRC})
set_target_properties(event PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})