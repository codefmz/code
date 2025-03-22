file(GLOB SRC "./*.cpp")

add_library(libproxy STATIC ${SRC})

set_target_properties(libproxy PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)