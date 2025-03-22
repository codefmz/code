file(GLOB SRC "./*.cpp")

add_library(libpointer STATIC ${SRC})

set_target_properties(libpointer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)