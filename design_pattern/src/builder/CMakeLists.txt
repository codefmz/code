file(GLOB SRC "./*.cpp")

add_library(libbuilder STATIC ${SRC})

set_target_properties(libbuilder PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)