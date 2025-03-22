file(GLOB SRC "./*.cpp")

add_library(libprototype STATIC ${SRC})

set_target_properties(libprototype PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)