cmake_minimum_required(VERSION 3.5)
project(ko_test)

#命令 uname -r 返回系统的内核版本信息
execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message("Kernel version: ${KERNEL_VERSION}")

set(KERNEL_SOURCE_DIR "/lib/modules/${KERNEL_VERSION}/build")

message(KERNEL_SOURCE_DIR = "${KERNEL_SOURCE_DIR}")

# 定义编译函数/方法，驱动编译过程主要在这个函数进行
function(compile_module obj)
    set(TARGET_NAME ${obj})

    # 添加目标，obj即为模块名称。由于我在build目录下构建，所以这里选择将.c和.h文件拷贝到该目录下
    add_custom_target(${TARGET_NAME} ALL
            COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND echo "compiling module ${obj}.ko..."
        )

    # 设置依赖，相当于Makefile中的 $(MODULE_NAME)-objs += demo_main.o
    #set(depend_objlist "ko_test_main.o")

    # 设置编译命令
    add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND echo "obj-m := ${obj}.o" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
            COMMAND make -C ${KERNEL_SOURCE_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules
            )
endfunction()

# 调用编译函数
compile_module(ko_test)

# 收集源文件
set(SOURCES
    main.c
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# 生成可执行文件
add_executable(koTest ${SOURCES})












