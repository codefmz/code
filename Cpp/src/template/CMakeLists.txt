file(GLOB SRC "./*.cpp")

add_library(libtemplate STATIC ${SRC})

set_target_properties(libtemplate PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)