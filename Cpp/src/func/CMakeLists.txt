file(GLOB SRC "./*.cpp")

add_library(libfunc STATIC ${SRC})

set_target_properties(libfunc PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)