aux_source_directory(${PROJECT_SOURCE_DIR}/self_dynamic/self_dynamic_div/src
                                            all_src_files)

set_property(GLOBAL PROPERTY SELF_DYNAMIC_DIV_INNER ${all_src_files}) #定义了一个全局的属性，属性的名字，属性的值是这个

