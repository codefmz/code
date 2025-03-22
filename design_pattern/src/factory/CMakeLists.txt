file(GLOB SRC "./*.cpp")

add_library(libfactory STATIC ${SRC})

set_target_properties(libfactory PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)