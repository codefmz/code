file(GLOB SRC "./*.cpp")

add_library(libobserver STATIC ${SRC})

set_target_properties(libobserver  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)