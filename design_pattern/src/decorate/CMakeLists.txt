file(GLOB SRC "./*.cpp")

add_library(libdecorate STATIC ${SRC})

set_target_properties(libdecorate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)