aux_source_directory(${PROJECT_SOURCE_DIR}/out_src OUT_POW)

include_directories(${PROJECT_SOURCE_DIR}/out_include)

add_library(out_static STATIC ${OUT_POW})
add_library(out_dynamic SHARED ${OUT_POW})

# 设置输出文件名
# set_target_properties(out_static PROPERTIES
#     OUTPUT_NAME "out_static__aa"  #自定义的名字，out_static 是目标的名字，没有起名默认用目标的名字
#     ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/out_static
# )

# set_target_properties(out_dynamic PROPERTIES
#     OUTPUT_NAME "out_dynamic__dd"
#     LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out_dynamic
# )

# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out_dynamic)
# 可选：指定安装路径
install(TARGETS out_dynamic DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS out_static DESTINATION ${PROJECT_SOURCE_DIR}/lib)