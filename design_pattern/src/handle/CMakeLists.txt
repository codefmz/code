file(GLOB SRC "./*.cpp")

add_library(libhandle STATIC ${SRC})

set_target_properties(libhandle PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)