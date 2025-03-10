file(GLOB SRC "./*.cpp")

add_library(libsingleton STATIC ${SRC})

set_target_properties(libsingleton  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)