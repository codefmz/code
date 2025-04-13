file(GLOB SRC "./*.cpp")

add_library(addr STATIC ${SRC})
set_target_properties(addr PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(addr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})