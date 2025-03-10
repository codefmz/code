file(GLOB SRC "./*.cpp")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_library(libcomponent STATIC ${SRC})

set_target_properties(libcomponent  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)