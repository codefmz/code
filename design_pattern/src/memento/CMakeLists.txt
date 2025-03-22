file(GLOB SRC "./*.cpp")

add_library(libmemento STATIC ${SRC})

set_target_properties(libmemento  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)