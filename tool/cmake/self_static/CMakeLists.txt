aux_source_directory(./self_static_add/src SELF_STATIC_ADD)
aux_source_directory(./self_static_sub/src SELF_STATIC_SUB)

include_directories(./comm
                    ./self_static_add/include
                    ./self_static_sub/include
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(self_static_lib STATIC ${SELF_STATIC_ADD} ${SELF_STATIC_SUB})
#这个模块构建生成一个静态库, STATIC 就表示一个静态库