file(GLOB SRC "./*.cpp")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_library(libvisit STATIC ${SRC})

set_target_properties(libvisit PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)