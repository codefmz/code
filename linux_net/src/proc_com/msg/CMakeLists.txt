file(GLOB SRC "./*.cpp")

add_library(libmsg STATIC ${SRC})

set_target_properties(libmsg PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)