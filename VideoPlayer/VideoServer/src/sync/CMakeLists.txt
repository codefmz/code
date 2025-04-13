file(GLOB SRC "./*.cpp")

add_library(sync STATIC ${SRC})
set_target_properties(sync PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(sync PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})