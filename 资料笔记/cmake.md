

# 面试题

## **基础问题**

### （1）**CMake 是什么？它的作用是什么？**

- **回答**：
  - CMake 是一个跨平台的构建工具，用于管理 C/C++ 项目的编译过程。
  - 它通过生成平台特定的构建文件（如 Makefile、Visual Studio 项目文件等）来实现跨平台构建。

### （2）**CMake 的基本工作流程是什么？**

- **回答**：
  1. 编写 `CMakeLists.txt` 文件，定义项目的构建规则。
  2. 运行 `cmake` 命令生成构建系统文件（如 Makefile）。
  3. 使用生成的构建系统（如 `make` 或 `ninja`）编译项目。

### （3）**如何编写一个最简单的 `CMakeLists.txt` 文件？**

- **回答**：

  ```
  cmake_minimum_required(VERSION 3.10)  # 指定 CMake 最低版本
  project(MyProject)                   # 定义项目名称
  add_executable(MyApp main.cpp)       # 添加可执行文件
  ```

------

## 2. **常用命令和变量**

### （1）**`add_executable` 和 `add_library` 的区别是什么？**

- **回答**：
  - `add_executable`：用于生成可执行文件。
  - `add_library`：用于生成库文件（静态库或动态库）。

### （2）**如何指定 C++ 标准？**

- **回答**：

  ```
  set(CMAKE_CXX_STANDARD 17)  # 使用 C++17 标准
  set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
  ```

### （3）**如何添加头文件目录和链接库？**

- **回答**：

  ```
  include_directories(${PROJECT_SOURCE_DIR}/include)  # 添加头文件目录
  target_link_libraries(MyApp PRIVATE mylib)         # 链接库
  ```

### （4）**`PRIVATE`、`PUBLIC` 和 `INTERFACE` 的区别是什么？**

- **回答**：
  - `PRIVATE`：仅对当前目标有效。
  - `PUBLIC`：对当前目标和依赖它的目标有效。
  - `INTERFACE`：仅对依赖它的目标有效。

------

## 3. **高级特性**

### （1）**如何实现条件编译？**

- **回答**：

  ```
  option(USE_FEATURE_X "Enable feature X" ON)  # 定义选项
  if(USE_FEATURE_X)
      add_definitions(-DUSE_FEATURE_X)        # 定义宏
  endif()
  ```

### （2）**如何跨平台编译？**

- **回答**：

  - 使用 `if` 语句判断平台：

    ```
    if(WIN32)
        # Windows 平台
    elseif(UNIX)
        # Unix/Linux 平台
    endif()
    ```

### （3）**如何生成静态库和动态库？**

- **回答**：

  ```
  add_library(MyStaticLib STATIC mylib.cpp)  # 静态库
  add_library(MySharedLib SHARED mylib.cpp)  # 动态库
  ```

### （4）**如何使用 `find_package` 查找外部库？**

- **回答**：

  ```
  find_package(OpenCV REQUIRED)  # 查找 OpenCV 库
  target_link_libraries(MyApp PRIVATE OpenCV::OpenCV)  # 链接 OpenCV
  ```

------

## 4. **项目实践**

### （1）**如何组织一个大型项目的 CMake 文件？**

- **回答**：

  - 使用 `add_subdirectory` 将项目划分为多个模块。

  - 每个模块有自己的 `CMakeLists.txt` 文件。

  - 示例：

    ```
    add_subdirectory(src)  # 添加 src 目录
    add_subdirectory(tests)  # 添加 tests 目录
    ```

### （2）**如何实现单元测试？**

- **回答**：

  - 使用 `enable_testing()` 和 `add_test()`：

    ```
    enable_testing()
    add_executable(MyTest test.cpp)
    add_test(NAME MyTest COMMAND MyTest)
    ```

### （3）**如何生成安装包？**

- **回答**：

  - 使用 `install` 命令：

    ```
    install(TARGETS MyApp DESTINATION bin)  # 安装可执行文件
    install(DIRECTORY include/ DESTINATION include)  # 安装头文件
    ```

------

## 5. **调试与优化**

### （1）**如何调试 CMake 脚本？**

- **回答**：

  - 使用 `message()` 打印调试信息：

    ```
    message(STATUS "Current value: ${VARIABLE}")
    ```

### （2）**如何优化 CMake 构建速度？**

- **回答**：
  - 使用 `ccache` 加速编译。
  - 减少不必要的依赖和文件扫描。
  - 使用 `ninja` 替代 `make` 作为生成器。

------

## 6. **常见问题排查**

### （1）**CMake 找不到库怎么办？**

- **回答**：
  - 确保库已安装，并正确设置 `CMAKE_PREFIX_PATH` 或 `PKG_CONFIG_PATH`。
  - 使用 `find_package` 或 `find_library` 手动指定库路径。

### （2）**如何解决 CMake 版本不兼容问题？**

- **回答**：

  - 在 `CMakeLists.txt` 中指定最低版本：

    ```
    cmake_minimum_required(VERSION 3.10)
    ```

  - 升级本地 CMake 版本。

------

## 7. **扩展知识**

### （1）**CMake 与 Modern CMake 的区别是什么？**

- **回答**：

  - Modern CMake 强调使用目标（target）为中心的语法，避免全局变量和命令。

  - 示例：

    ```
    target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(MyApp PRIVATE mylib)
    ```

### （2）**如何编写跨平台的 CMake 脚本？**

- **回答**：
  - 使用 `if` 判断平台。
  - 使用 `find_package` 查找平台特定的库。
  - 避免硬编码路径，使用 CMake 提供的变量（如 `CMAKE_INSTALL_PREFIX`）。

## 交叉编译



# gcc指令

## 编译流程

### 预处理

1. 处理头文件，宏

### 编译

1. 将原代码翻译成为机器可执行代码的中间产物，与机器相关，包含了机器指令，数据段，符号表。

### 汇编

### 链接

1. 将多个目标文件以及库文件合并为一个可执行程序

---

## 常用指令

### -g

### -O

![image-20231005162744832](./cmake.assets/image-20231005162744832.png)

### -o

目标名字

### -c

### -L

指定库文件路径

### -l

指定库文件名字

### -Wall

打印警告消息

### -w

关闭警告消息

### -std=c++11

设置编译标准

### -D

定义宏

---

## 库

### 静态库

#### 生成

1. 目标文件

   ```bash
   gcc API.c  -Iinclude -o API.o
   ```

2. 链接目标文件生成静态库

   ```bash
   ar rs staticlibAPI.lib API.o
   ```

#### 使用

```bash
gcc main -Iinclude -Llib -lstaticAPI -o staticAPI.exe
```

---

### 动态库

#### 生成

1. 生成dll文件，但是不生成lib文件

   ```bash
   gcc API.c -Iinclude -fPIC -shared -o DyLIb.dll
   ```

   > 生成的dll文件不能直接使用， 需要设置临时工作环境 $env:PATH="lib;" + $env:PATH, 然后直接跑

2. 生成dll文件与lib文件

   ```bash
   gcc API.c -Iinclude -fPIC -shared -o DyLIb.dll -Wl,--out-implib,libDyLIb.lib
   ```

#### 使用

```bash
gcc -o my_program my_program.c -Ipath/to/include -Lpath/to/lib -lDyLIb
```

---

# Cmake

## 背景

业务复杂导致的源码构建工具， linux原本使用`Makefile`,但是`Makefile`太难用了

## 基础语法

### 最低版本号

```shell
cmake_minimum_required(VERSION 3.5.0)
```

### 项目名称与版本

```shell
project(DataStruct VERSION 0.1.0)
 ${PROJECT_NAME} 可以使用
```

### 注释

```cmake
#
#[[
多行注释
]]
```

### 打印

```shell
message(" helllo world : ${PROJECT_NAME}")
```

### 定义变量

```shell
set、option
```

> option 通常用于定义`bool`类型的变量

### 变量引用

```cmake
 ${VARIABLE_NAME}
```

### 条件语句

```cmake
if()
else if()
else()
endif()
```

> 使用变量if里面不需要使用${}

### 字符比较

```cmake
STREQUALL()
```

### 子项目

```cmake
add_subdirectory(subDir)
```

> 指定子目录， 子目录必须包含 CMakeLists.txt

### 构建可执行文件

```cmake
add_executable(target main.c)
```

### 源文件列表

```cmake
aux_source_directory(./src UTIL_SRC)
```

> 将目录中的源文件收集起来赋值给 UTIL_SRC

### 构建库

```cmake
add_library(util_static STATIC ${UTIL_SRC}) #静态库

add_library(util_static SHARED ${UTIL_SRC}) #动态库

```

### 指定目标依赖头文件

```cmake
target_include_directories(util_static
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

### 构建目标同时执行指令

```cmake
add_custom_target(list
    COMMAND echo "build LIST start..."
    COMMAND rm -rf ./LIST
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target LIST
    COMMAND cp /usr/fmw/C/DataStruct_Alg/bin/LIST /usr/fmw/C/DataStruct_Alg/LIST
    COMMAND echo "build LIST end..."
)
```

> 添加自定义指令， 使用 make  指令执行 ：make target
>
> 在构建目标前后执行此目标



## 关键字

###  PROJECT_NAME

###  PROJECT_SOURCE_DIR

> cmakeList.txt 项目的目录，

### CMAKE_CURRENT_SOURCE_DIR

> cmake 文件所在的路径，可以是子文件的路径

### PROJECT_BINARY_DIR

### CMAKE_LIBRARY_OUTPUT_DIRECTORY 

> 指定 lib 文件输出目录

---

## 静态库

1. 如何连接动态库?

   - 指定动态库寻找路径

     ```cmake
     link_directories(
         ${PROJECT_SOURCE_DIR}/lib
         ${PROJECT_SOURCE_DIR}/lib/util/
         ${PROJECT_SOURCE_DIR}/lib/gtest/
     )
     ```

   - 指定需要链接的动态库

     ```cmake
     add_executable(ringbuffer_test test/DataStruct/ringbuffer_test.cc)
     target_link_libraries(ringbuffer_test rb_static util_static gtest_main gtest)
     ```

2. 指定可执行文件输出路径

   ```cmake
   set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
   ```

3. 指定编译选项带pthread， 可以调试

   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")
   ```

4. 指定生成lib的文件夹

   ```cmake
   set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
   ```


---

# sh脚本

```sh
if [ $# != 1 ];
then
    echo "Invalid argument!"
    exit 1
fi

MakeProject=$1
echo "tttt = $MakeProject"
cd ./build/
cmake -DVAR_NAME="$MakeProject" ..
echo "project $0 make $1"
make $1
rm -rf ./*
```

_DVAR_NAME : 用于在执行时给`CMake`一个变量，并将其传递给`CMakeList.txt`文件

cmake中

1. 检查文件是否存在

   ```bash
   if [ -e "lib/util/" ] && [ -e "include/util/" ]; then
       echo "util exist "
   else
       sh ../Util/build.sh $script_dir
   fi
   ```

2. 获取sh脚本所在的文件夹

   ```bash
   script_dir="$(cd "$(dirname "$0")" && pwd)"
   ```

# windows编译sln文件

```bash
cmake -G "Visual Studio 16 2019" .
```

# 编译指令

```cmake
cmake -S . -B build
cmake --build build --target t //指定编译目标为t
```



```c
#semaphare
add_executable(Semaphare main.cpp)
target_link_libraries(Semaphare gtest semaphare)
target_link_options(Semaphare PRIVATE "/WHOLEARCHIVE:semaphare.lib")
add_custom_command(TARGET Semaphare
    COMMAND echo "compile semaphare test..."
)

#string_test
add_executable(t_string main.cpp)
target_link_libraries(t_string gtest string_test)
target_link_options(t_string PRIVATE "/WHOLEARCHIVE:string_test.lib")
add_custom_command(TARGET Semaphare
    COMMAND echo "compile semaphare test..."
)
```

# 设置编译选项

```cmake
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library 与 add_executable 前设置，msvc的编译选项为utf-8， msvc编译器默认编译的文本是utf-8-bom 会识别为utf-8

chcp 65001 //修改控制台的编码为utf-8
```

