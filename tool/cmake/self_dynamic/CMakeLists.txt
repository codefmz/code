add_subdirectory(self_dynamic_div)
add_subdirectory(self_dynamic_mul/src)

get_property(SELF_DYNAMIC_DIV GLOBAL PROPERTY "SELF_DYNAMIC_DIV_INNER") #获取全局的属性
get_property(SELF_DYNAMIC_MUL GLOBAL PROPERTY "SELF_DYNAMIC_MUL_INNER")

include_directories(./comm)

message("SELF_DYNAMIC_DIV = :" ${SELF_DYNAMIC_DIV})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_library(self_dynamic_dll SHARED ${SELF_DYNAMIC_DIV} ${SELF_DYNAMIC_MUL})
#这个模块构建生成一个动态库, SHARED 就表示一个动态库