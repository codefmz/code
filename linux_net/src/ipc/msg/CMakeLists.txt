file(GLOB SRC "./*.cpp")

add_library(libmsg STATIC ${SRC})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(libmsg PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)