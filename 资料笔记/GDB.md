



# 面试

## gdb是什么原理？

gdb 主要是通过ptrace 与 信号来生效的，被调试程序是gdb的子进程，通过SIGCHILD信号来与子进程通信。通过ptrace来查看子进程的寄存器信息。

-g 

---



## 程序启动指令

./gdb  进程

b 断定

run

start

也可以使用gdb attach pid 映射上已经在运行的程序

---

## 怎么中断

exit

## 退出

quit

---

## 基本指令

b

i b

n

finish

s

u

list 

bt

info locals

**程序如果想要调试，需要带 `-g`编译选项**

```bash
gcc main.c -g3 -lpthread -o main
set(CMAKE_C_FLAGS "-g -std=c99 -pthread ${CMAKE_C_FLAGS}")
```

> -g 生成包含行号、变量名、函数名等基本调试信息
>
> -g2 包含上面，内联函数不会展开，可以GDB进去
>
> -g3 包含上面，宏不会替换
>
> 

回车重复前一条指令

tab键可以补全代码

# 实现原理

## 信号

### 本质

信号是在软件层次上对中断机制的一种模拟，在原理上，一个进程收到一个信号与处理器收到一个中断请求可以说是一样的。信号是一异步的，一个进程不必通过任何操作来等待信号的到达。信号是进程间通信机制中唯一的异步通信机制，可以看作是异步通知，通知接受信号的进程以后那些事情发生了。信号机制经过POSIX实时拓展后，功能更加强大，除了基本通知功能外，还可以传递附加信息。

### 来源

- 硬件来源 ： 按下键盘、或者其他硬件故障灯
- 软件来源 ： 发送系统函数， kill, raise, alarm 和 settimer 以及 sigqueue 函数

### 响应

1. 忽略信号： 不对信号做任何处理，SIGKILL与SIGSTOP这两个信号不能忽略。
2. 捕捉信号： 定义信号处理函数
3. 执行缺省: 交由系统按照默认处理，终止或者忽略信号

**GDB主要用到了SIGCHILD 信号**

## 软件跟踪

GDB实现主要依赖 **ptrace** 系统调用以及信号机制。

ptrace 系统调用提供了一种方法来让父进程可以观察和控制其它进程的执行，检查和改变其核心映像以及寄存器。而信号则提供了通知响应机制。

## 软件断点

打断点时，GDB会将程序对应的位置的指令替换为brk指令，待程序运行到此处时，会触发SIGTRAP异常信号，并由GDB接管处理，从而实现断点功能。

## 数据断点 watch point

1. GDB将被观察的数据地址写入到DBGWVR观察值寄存器中；
2. 处理器执行程序指令时，比较访问数据地址是否与DBGWVR匹配，将会触发观察点异常。

# 启动

- gdb <program> 启动进入调试
- gdb <program> <core> 调试程序core文件
- gdb <program> <pid> 调试指定PID的运行中程序

# 运行

- start <args> 启动调试程序并停在主程序入口处
- run <args> 启动程序直接运行

# 中断

ctrl + C  : 运行程序后，可中断正在运行的程序回到GDB命令端

# shell

- shell 可以退出gdb 到shell终端，exit可以退出
- shell <command> 在GDB中执行shell命令

# 退出

quit 结束GDB调测，退出到终端

# 帮助

- help 列举命令
- help all
- help <command>
- apropos <word> 查看word关键字相关的命令
- complete <prefix> 查看prefix 

# 日志输出

记录每一条输入输出调试记录， 默认关闭

- set logging on 激活日志
- set logging off 关闭日志
- set logging file <filename> 改变当前日志输出目标文件，默认是gdb.txt

# 查看源码

- list
- list \<linenum\> 指定行号的代码
- list  \<begin\>，\<end\> 
- list <+/- offset> 正负偏移量
- list \<file:linenum\> 指定文件指定行号
- list \<func\> 查看指定函数
- list \<file:func\>
- list * \<address\> 查看程序运行时在指令地址的源码语句
- show list 当前显示源代码的行数
- set list \<num\>

# 查看汇编

- disassembl，简写:disas
- disas \<func\> 查看函数反汇
- disas 0x\<hhhhhhh\> 函数指令地址的函数反汇编

# 断点

## 指令断点

**设置断点**

- break \<linenum> 
- b \<fun\>
- b \<file:line\>
- b \<file:func\>
- b *\<address\>

**查看断点**

- info break 简写 i b
- info break n 指定断点

## 数据断点

3种 

> - watch ： 被写时
> - rwatch ： 被读时
> - awatch : 被读写时

- watch \<expr\> 监视变量
- watch *\<address\> j监视指定地址的内存
- info watchpoint 查看断点，

**断点管理同指令断点**

## 管理断点

- delete n 删除断点

- disable n 禁用断点

- enable n 恢复断点

- clear expr 

  > clear main
  >
  > clear main.c:119

## 断点打印

- display \<expr\> 每次断点都会自动打印expr信息
- undisplay 取消前面使能的display

## 条件断点

-  condition \<bnum> \<expr> 设置条件断点，满足条件时, 用于当断点已经存在时
- b \<location> if \<expr>  打断点时指定条件，创建并设置条件
- ignore \<bnum> \<count\> 忽略断点执行次数

## 断点命令

commands \<bnum\> 为bnum 断点写入命令列表，当程序执行到该断点时，将会依序执行列表命令

[...command - 1....]

[...command - 2....]

[...command - 3....]

...

end

# 调用栈

- backtrace 查看当前调用栈， 简写bt

- frame \<n> 进入某层函数的栈现场
- up \<n> 向栈上面移动n层
- down \<n> 向栈下面移动n层
- bt full 查看所有调用者栈，入参，函数变量等

# 栈帧空间数据

- info args 当前函数的参数名及其值
- info locals 当前函数所有局部变量及其值
- info register 查看当前线程所有寄存器值
- info frame \<n> 查看当前线程的所有寄存器值
- whatis \<var>  显示变量的类型
- show args 显示程序入口参数

# 解析内存数据

- print \<expr> 查看变量 简写： p
- p & \<expr> 查看变量的地址信息
- p /\<f\> \<expr\> 按指定格式去查看变量值
- p \*(struct*)（var)  可以使用类型强转
- p *array@10 可以打印数组10个值
- x /<n/u/f> \<addr\> 指定长度格式打印内存数据 
- set var=10 修改某个变量的值
- show env 查看环境变量
- set env 设置环境变量

# 程序运行控制

- continue
- continue <ignore-count> 屏蔽掉指定断点次数
- next 单步执行，越过函数调用
- next \<count> 执行后面的count条语句，然后停住
- step 函数调用，步入函数内
- step <count> 执行count 条指令，然后挺住
- set step-mode on 单步跟踪，不会因为没有debug信息而停不住
- stepi [num] 以汇编指令的粒度进行单步运行，可用num指定步数，简写： si
- nexti [num] 同上，不进入调用函数，简写：ni
- until <line> 执行到指定line然后停住，简写：u
- finish 运行到函数结束，简写：fin
- jump <line> 跳转到指定行数，用来打空函数
- jump \<file:line\> 跳转到指定的文件名和行号继续执行
- jump + <num> 
- jump *<address> 跳转到指定指令地址已婚性
- return <expression> 强制当前函数返回，可以结合断点打空函数
- call <func> 强制调用函数
- handle <SIGNAL> <Actions...> 设置GDB信号处理，如果有多个ACTION可使用空格隔开
- signal <SIGNAL> 使用GDB命令向被调试程序发送信号
- info signals 查看有那些信号被GDB检测

# 多线程

1. 显示线程

   ```bash
   info threads
   ```

2. 跳转到指定线程

   ```bash
   thread 1
   ```

3. 指定线程执行 command 指令

   ```bash
   thrad apply 1 2 bt
   thread apply all command 
   ```

4. 指定线程`break`

   b <expr> thread <thread_num> 只给 thread_num 这个线程的 xxxx函数打断点

   ```bash
   b 110 thread 1
   ```

5. 设置调度锁定

   set scheduler-loking <off|on|step> 

   off 不锁定任何线程，也就是所有线程都执行，默认值

   on 只有当前被调试线程执行

   step 其他线程不会执行，但是当其他命令，next调试时，其他线程也许会执行

---

# 多进程

1.  ```bash
   gdb attach pid
   ```
   
2. fork子进程调试

   ```bash
   set fork-on-detach on/off  #子进程分离后，gdb是否接管它的调试
   set follow-fork-mode child/parent  #设置fork后跟随主进程还是子进程
   gdb -ex 'set follow-fork-mode child' ./your_program #可以在启动时设置
   ```

3. 显示所有正在调试的进程

   ```bash
   info inferiors
   ```

4. 跳转到指定进程

   ```bash
   inferior 1
   ```

----



# GDB拓展能力

## 自定义命令

```shell
define <cmdname>
	<expr...>
end
```

> 定义一个命令，其动作将会遵循expr的逻辑来执行

## 条件表达式

```shell
if <cond-expr>
...
else
...
end
```

## 循环表达式

```shell
while <cond-expr>
...
end
```

## 定义命令文档

```shell
document <cmdname>
...
end
```

> 为自定义的命令提供帮助文档，可以通过help cmdname 来查看定义信息

## 命令表达式参数

```shell
$arg0...$arg9 表示命令表达式参数，最多10个
```

## 例子

```c
typedef struct {
    int cnt;
    int mode;
    int *storeys;
} LIFT;
```

```shell
define pargs
	echo cnt:\n
 	x /1wd $arg0
 	echo mode:\n
 	x /1wd ($arg0+4)
 	set $i=0
 	set $limit=*(int *)($arg0)
 	set $array=*(int *)($arg0+8)
 	echo storeys:\n
 	while $i < $limit
 		x /1wd $array+($i * 4)
 		set $i = $i + 1
 	end
 end
```

写到.gdbinit 文件中去，启动gdb时会自动从.gdbinit 文件中读取自定义命令



# 典型问题实战

## core文件

### 产生

core文件是当操作系统接受到了程序无法处理异常信号，会将程序的线程栈，内存信息保存起来进行快照，即俗称的coredump。

```shell
ulimit -c #查看是否可以生成coredump文件
ulimit -c unlimited #设置coredump文件无限制
echo "/tmp/core.%e.%p" | sudo tee /proc/sys/kernel/core_pattern #控制coredump文件的路径
```



### 程序的概念

源码-》程序-》进程-》线程

### 程序运行过程

1. 加载程序，将指令与数据映射到内存空间中去，包括加载链接所需要的库；每个动态库只会在第一次加载占用实际的物理内存，后续进程使用这个动态库都只是去映射这块物理地址。
2. 为程序初始化一个启动栈空间，并将参数整理加载到寄存器中
3. 自main函数开始执行程序指令。

### 内存地址空间

![image-20250108155750356](./GDB.assets/image-20250108155750356.png)



1. 64位CPU实际有效寻址为宽慰48位
2. 内核空间除有效寻址位宽外，前面高位全为1
3. 用户空间除有效寻址位宽外，前面高位全为0

### 环境寄存器

| 分类       | 寄存器  | 作用                                                         |
| ---------- | ------- | ------------------------------------------------------------ |
| 通用寄存器 | X0-X7   | 传递参数用，超过8个参数的部分，保存在调用栈中，通过栈传递给被调用函数，x0还作为返回值寄存器使用，保存立即数或作为临时寄存器使用 |
|            | X8      | 间接结果寄存器，一般用来传递间接结果的地址，比如当函数返回一个大的结构体时 |
|            | X9-X15  | 在程序中一般作为局部变量寄存器使用，调用子函数前需要先保存   |
|            | X16-X17 | ipc 寄存器，常用于存储内部过程调用的临时跳转地址             |
|            | X18     | 平台预留寄存器                                               |
|            | X19-X29 | 谁使用谁恢复，被调用函数使用前需要先保存，被调用函数返回前需要恢复 |
|            | X30     | 又名LR，保存函数调用的返回地址，用户函数返回时继续操作       |
| 特殊寄存器 | SP      | 栈地址寄存器，保存当前任务的栈顶地址，函数开栈后的栈地址必须16字节对齐 |
|            | PC      | 程序计数寄存器，保存当前指令的地址，计算相对偏移是，该地址不需要像ARM32架构那样+8 |

> X0 - X64 都是64bit的寄存器
>
> W0 - W30表示的是32bit长度，复用了X0 - X30的低32bit



### 汇编 - 后续补充

### 堆栈结构

栈中保存顺序从低地址到高地址为： stack args（栈参数，超过8个的参数通过栈来传参)、fp、lr、callee save(用来保存易失寄存器的值)、local(局部变量)。

## 段错误问题

本质是程序试图访问未被允许的内存区域，例如访问未被分配内存，已释放的内存或者尝试写入只读内存

### 定位流程

```shell
gdb  main core #应用程序 + core文件， 
export LIFT_MODE=1
gcc -O2 main.c -lpthread -o main
./main 1 2 3 3 0 5 6

```

![image-20250108192048289](./GDB.assets/image-20250108192048289.png)

```shell
info registers #查看寄存器信息，pc寄存器，x86是rip
```

![image-20250108192305049](./GDB.assets/image-20250108192305049.png)

对rip寄存器进行反汇编

```shell
disass 0x400fab
```

![image-20250108192427202](./GDB.assets/image-20250108192427202.png)

结合源码分析具体定位发送错误的位置

## 内存破坏问题

- 越界访问：程序访问了超出其合法范围的内存区域。
- 双重释放：同一块内存被释放了多次，导致内存管理器状态不一致
- 释放后访问：
- 栈溢出
- 缓冲区溢出

```c
export LIFT_MODE=1
./main 10 20 30 40 50 60 70 80 90   
```

### 定位流程

1. 如果内存破话地址比较有规律可以通过watchpoint定位

2. 确定内存内存破坏的地址，调用 make_mprotect 设置内存为只读

   ![image-20250108214751053](./GDB.assets/image-20250108214751053.png)

3. 运行程序，内存破坏写的地方会触发段错误

   ![image-20250108215007907](./GDB.assets/image-20250108215007907.png)

4. 查看寄存器信息, 确定rip寄存器或者pc寄存器的值，该值为当前执行的指令地址

   ```shell
   info registers
   ```

   ![image-20250108215108095](./GDB.assets/image-20250108215108095.png)

5. 确定当前执行指令为 mov, 将bl寄存器存储的值放到，0x604120这个地址偏移%rcx， 并结合源码分析是那一行代码出现的问题

![image-20250108222304677](./GDB.assets/image-20250108222304677.png)

6. 打印一下看是那个变量

![image-20250108222602906](./GDB.assets/image-20250108222602906.png)

7. 打印rcx寄存器的值，获取偏移量为8010， 其值大于g_rand_map数组的容量

   

   

   

## 死锁问题

死锁必要条件

1. 互斥条件
2. 持有并等待
3. 不剥夺条件
4. 循环等待条件

```shell
 gcc -g3 main.c -lpthread -o main
 export LIFT_MODE=3
 ./main 1 2 3 5 6 3 &
```

### 观察现象

1. 长时间停止无输出
2. 命令top -p <PID> -H 观察程序，CPU的占用率持续为0

![image-20250108231431031](./GDB.assets/image-20250108231431031.png)

3. 命令 cat /proc/15413/stack 查看进程内核调用栈信息,可以看到在等待队列中

![image-20250108232427914](./GDB.assets/image-20250108232427914.png)

4.通过gdb attach 进程，获取调用栈，打印锁的值，判断当前等待资源被谁占用了，推断等待链

![image-20250108232839646](./GDB.assets/image-20250108232839646.png)
