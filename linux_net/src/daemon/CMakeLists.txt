file(GLOB SRC "./*.cpp")

add_library(libdaemon STATIC ${SRC})
message("daemon ======")

set_target_properties(libdaemon PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)