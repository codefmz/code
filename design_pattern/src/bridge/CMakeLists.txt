file(GLOB SRC "./*.cpp")

add_library(libbridge STATIC ${SRC})

set_target_properties(libbridge PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)