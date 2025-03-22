file(GLOB SRC "./*.cpp")

add_library(libregex STATIC ${SRC})

set_target_properties(libregex PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)