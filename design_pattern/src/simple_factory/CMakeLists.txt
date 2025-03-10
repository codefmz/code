file(GLOB SRC "./*.cpp")

add_library(libsimple_factory STATIC ${SRC})

set_target_properties(libsimple_factory PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)