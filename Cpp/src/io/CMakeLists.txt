file(GLOB SRC "./*.cpp")

add_library(libio STATIC ${SRC})

set_target_properties(libio PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)