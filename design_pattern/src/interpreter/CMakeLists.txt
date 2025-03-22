file(GLOB SRC "./*.cpp")

add_library(libinterpreter STATIC ${SRC})

set_target_properties(libinterpreter PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)