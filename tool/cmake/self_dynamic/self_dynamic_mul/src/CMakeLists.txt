aux_source_directory(${PROJECT_SOURCE_DIR}/self_dynamic/self_dynamic_mul/src
                                            all_src_files)

set_property(GLOBAL PROPERTY SELF_DYNAMIC_MUL_INNER ${all_src_files})