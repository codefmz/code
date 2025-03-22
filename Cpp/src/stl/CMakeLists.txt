file(GLOB SRC "./*.cpp")

add_library(libstl STATIC ${SRC})

set_target_properties(libstl PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)