file(GLOB SRC "./*.cpp")

add_library(libstrategy STATIC ${SRC})

set_target_properties(libstrategy PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)