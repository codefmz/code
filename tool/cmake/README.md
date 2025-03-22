## 项目1

CMake 从工程目录下的CMakeLists.txt 开始处理，根据上一个cmake的配置，依次处理内部的CMakeLists.txt

```bash
├── bin
├── build
├── CMakeLists.txt
├── config
│   └── config.h.in
├── main.cpp
├── out_dynamic
├── out_include
│   └── out_include_pow.h
├── out_src
│   ├── CMakeLists.txt
│   └── out_include_pow.cpp
├── out_static
├── self_dynamic
│   ├── CMakeLists.txt
│   ├── comm
│   │   └── dynamic_comm.h
│   ├── self_dynamic_div
│   │   ├── CMakeLists.txt
│   │   ├── include
│   │   │   └── self_dynamic_div.h
│   │   └── src
│   │       └── self_dynamic_div.cpp
│   └── self_dynamic_mul
│       ├── include
│       │   └── self_dynamic_mul.h
│       └── src
│           ├── CMakeLists.txt
│           └── self_dynamic_mul.cpp
└── self_static
    ├── CMakeLists.txt
    ├── comm
    ├── self_static_add
    │   ├── include
    │   │   └── self_static_add.h
    │   └── src
    │       └── self_static_add.cpp
    └── self_static_sub
        ├── include
        │   └── self_static_sub.h
        └── src
            └── self_static_sub.cpp
```

### 目录概述

外部依赖库（功能为pow， 见out_src)

out_dynamic：外部依赖的动态库目录

out_static：外部依赖的静态库目录

out_include：外部依赖的头文件

内部功能模块

self_dynamic：包含div、mul两个功能模块

self_static：包含add、sub两个功能模块

config：构建配置

main.cpp：主程序main函数

bin：构建文件生成目录

build：构建临时目录（外部构建）

外部构建：构建的结果不污染整个工程

---

### 构建流程

1. 设置编译选项
2. 指定构建配置
3. 指定头文件
4. 生成静态库
5. 生成动态库
6. 指定外部lib库
7. 生成可执行目标文件
8. 指定文件存放目录

---

### 各级CMakeList.txt

#### 根目录

```cmake
#message("PROJECT_SOUCE_DIR = " ${PROJECT_SOURCE_DIR})
# message("CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})

include_directories(./out_include
                    ./self_static/self_static_add/include
                    ./self_static/self_static_sub/include
                    ./self_dynamic/self_dynamic_div/include
                    ./self_dynamic/self_dynamic_mul/include
                    ./config
)

##[[
add_subdirectory(self_static)
add_subdirectory(self_dynamic)
add_subdirectory(out_src)

link_directories(./out_static ./out_dynamic)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} main.cpp)


option(USE_MY_STATIC_OUT "option" ON) #如果变量定义过，则不处理，如果未定义过，则设置默认值 “option” 是一个描述符
option(TEST_CMAKE_DEFINE_OFF_EFFECT "comment" OFF)
set(TEST_VARIABLE_VALUE "\"TEST_VARIABLE_NAME IS WAHAHA\"")

configure_file (
    "${PROJECT_SOURCE_DIR}/config/config.h.in"
    "${PROJECT_SOURCE_DIR}/config/config_my89757.h"
)

message("USE_MY_STATIC_OUT = :" ${USE_MY_STATIC_OUT} "CMAKE_INSTALL_PREFIX = " ${CMAKE_INSTALL_PREFIX})

if(${USE_MY_STATIC_OUT} STREQUAL "ON")
target_link_libraries(${PROJECT_NAME} out_static self_static_lib self_dynamic_dll)
else()
target_link_libraries(${PROJECT_NAME} out_dynamic self_static_lib self_dynamic_dll)
endif()
#]]

# add_library(total_dll SHARED out_static self_static_lib self_dynamic_dll)
```

#### out_src

```cmake
aux_source_directory(${PROJECT_SOURCE_DIR}/out_src OUT_POW)

include_directories(${PROJECT_SOURCE_DIR}/out_include)

add_library(out_static STATIC ${OUT_POW})
add_library(out_dynamic SHARED ${OUT_POW})

# 设置输出文件名
set_target_properties(out_static PROPERTIES
    OUTPUT_NAME "out_static__aa"  #自定义的名字，out_static 是目标的名字，没有起名默认用目标的名字
    ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/out_static
)

set_target_properties(out_dynamic PROPERTIES
    OUTPUT_NAME "out_dynamic__dd"
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out_dynamic
)

# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out_dynamic)
# 可选：指定安装路径
# install(TARGETS out_dynamic_so DESTINATION ${PROJECT_SOURCE_DIR}/out_dynamic)
# install(TARGETS out_static_a DESTINATION ${PROJECT_SOURCE_DIR}/out_static)
```

#### self_dynamic

```cmake
add_subdirectory(self_dynamic_div)
add_subdirectory(self_dynamic_mul/src)

get_property(SELF_DYNAMIC_DIV GLOBAL PROPERTY "SELF_DYNAMIC_DIV_INNER") #获取全局的属性
get_property(SELF_DYNAMIC_MUL GLOBAL PROPERTY "SELF_DYNAMIC_MUL_INNER")

include_directories(./comm)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_library(self_dynamic_dll SHARED ${SELF_DYNAMIC_DIV} ${SELF_DYNAMIC_MUL})

#这个模块构建生成一个动态库, SHARED 就表示一个动态库
```

##### self_dynamic_div

```cmake
aux_source_directory(${PROJECT_SOURCE_DIR}/self_dynamic/self_dynamic_div/src
                                            all_src_files)
set_property(GLOBAL PROPERTY SELF_DYNAMIC_DIV_INNER ${all_src_files}) #定义了一个全局的属性，属性的名字，属性的值是这个
```

##### self_dynamic_mul\src

```cmake
aux_source_directory(${PROJECT_SOURCE_DIR}/self_dynamic/self_dynamic_mul/src
                                            all_src_files)
set_property(GLOBAL PROERITY SELF_DYNAMIC_MUL_INNER ${all_src_files})
```

#### self_static

```cmake
#[[
set(SELF_STATIC_SRC ./self_static_add/src/self_static_add.cpp
                    ./self_static_sub/src/self_static_sub.cpp
)
]]

aux_source_directory(./self_static_add/src SELF_STATIC_ADD)
aux_source_directory(./self_static_sub/src SELF_STATIC_SUB)

include_directories(./comm
                    ./self_static_add/include
                    ./self_static_sub/include
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_library(self_static_lib STATIC ${SELF_STATIC_ADD} ${SELF_STATIC_SUB})

#这个模块构建生成一个静态库, STATIC 就表示一个静态库
```



> **并不是每个文件夹都需要一个CMakeLists.txt文件，根据需要确定，比较灵活，这里只是演示**

---

### config.h.in 文件

配置文件，一般用来配置宏，option 中为`ON`的这里会定义

```c
#ifndef CONFIG_H
#define CONFIG_H

#cmakedefine USE_MY_STATIC_OUT
#cmakedefine TEST_CMAKE_DEFINE_OFF_EFFECT
#define TEST_VARIABLE_NAME ${TEST_VARIABLE_VALUE}

#endif
```

