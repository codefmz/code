file(GLOB SRC "./*.cpp")

add_library(add STATIC ${SRC})

set_target_properties(add PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)