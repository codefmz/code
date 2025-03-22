#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>

#define MAX_MAPS 4096
#define MAX_ZERO 8192
#define MAX_THREAD 3
#define MAX_FLOORS 100

typedef struct lift_args {
    int cnt;
    int mode;
    int *storeys;
} LIFT;

int g_thread_finish_cnt = 0;
int g_child_proc_exit = 0;
int g_lift_trace_cnt = 0;
int *g_temp_ptr = NULL;
int *g_lift_trace[MAX_THREAD] = { 0 };
char g_sign_map[MAX_ZERO] = { 0 };
char g_rand_map[MAX_MAPS] = { 0 };
char g_mark_map[MAX_ZERO] = { 0 };
pthread_mutex_t g_lift_mutex[MAX_FLOORS];

/* 检查预留变量的完整性 */
void check_maps()
{
    int i;
    int sign_cnt = 0;
    int mark_cnt = 0;

    for (i = 0; i < MAX_ZERO; i++) {
        if (g_sign_map[i] != 0) {
            sign_cnt++;
        }
        if (g_mark_map[i] != 0) {
            mark_cnt++;
        }
    }

    printf("check_maps: sign_cnt = %d, mark_cnt = %d\n", sign_cnt, mark_cnt);
}

int str2Digit(char *str)
{
    int ret = 0;

    if (strspn(str, "0123456789") == strlen(str)) {
        ret = atoi(str);
        if (ret < MAX_FLOORS) {
            return ret;
        }
    }

    return -1;
}

/* 信号处理函数, 构造信号的可观察性以及控制子进程退出 */
void signal_handler(int signo)
{
    pthread_t tid = pthread_self();
    printf("signal_handler: tid = %lu, signo = %d\n", tid, signo);
    g_child_proc_exit = 1;
}

/* 随机打乱数组 */
void shuffle(int cnt, int *storeys)
{
    int i, ri, tmp;

    for (i = 0; i < cnt; i++) {
        tmp = storeys[i];
        ri = rand() % cnt;
        storeys[i] = storeys[ri];
        storeys[ri] = tmp;
    }
}

void operate_enter(int mode, int prev)
{
    if (mode == 3) {
        pthread_mutex_lock(&g_lift_mutex[prev]);
    }
}

/* 模拟业务运行 */
void operate_run(int mode, int prev, int next)
{
    int tmp;
    if (mode == 3) {
        sleep(1);
        pthread_mutex_unlock(&g_lift_mutex[next]);
        pthread_mutex_lock(&g_lift_mutex[prev]);
    } else {
        g_temp_ptr = malloc(1024 * 100 * prev);
        if (g_temp_ptr != NULL) {
            g_temp_ptr = (int *)((char *)g_temp_ptr + 102400 - 1024);
            *g_temp_ptr = prev;
        }

        tmp = rand() % 120;
        g_rand_map[next * tmp] = next;
        sleep(1);
    }
}

/* 结束前清理 */
void operate_exit(int mode, int next)
{
    if (mode == 3) {
        pthread_mutex_unlock(&g_lift_mutex[next]);
    }
}

/* 以打印轨迹的方式构造模拟电梯运行 */
void *lift_entry(void *args)
{
    int i, j, prev, next, loop;
    int *storeys;
    pthread_t tid;
    LIFT *liftArgs = (LIFT *)args;

    if (liftArgs == NULL || liftArgs->storeys == NULL) {
        return NULL;
    }

    /* 并发模式增加循环量主要是为了延长程序运行便于观察 */
    loop = (liftArgs->mode != 0) ? 300 : liftArgs->cnt;
    tid = pthread_self(); /* 通过线程ID区分不同线程轨迹 */
    prev = liftArgs->storeys[0];
    printf("===========> LITF[%lu] begin in %d \n", tid, prev);
    operate_enter(liftArgs->mode, prev);
    for (j = 1; j < loop; j++) {
        i = j % liftArgs->cnt;
        if (prev == liftArgs->storeys[i]) {
            next = prev;
            printf("===========> LITF[%lu] begin in %d \n", tid, next);
        } else {
            next = liftArgs->storeys[i];
            printf("===========> LITF[%lu] begin in %d \n", tid, next);
        }

        operate_run(liftArgs->mode, prev, next);
        prev = next;
    }

    operate_exit(liftArgs->mode, next);
    g_thread_finish_cnt++;
    check_maps();

    return NULL;
}

/* 初始化资源 */
int init_res()
{
    int i;
    pthread_mutexattr_t mattr;

    for (i = 0; i < MAX_FLOORS; i++) {
        pthread_mutexattr_init(&mattr);
        /* 支持锁重入 */
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
        if (pthread_mutex_init(&g_lift_mutex[i], &mattr) != 0) {
            printf("mutex init failed!\n");
            return -1;
        }
    }

    return 0;
}

/* 解析入参数据 */
int *parse_args(int argc, char *argv[])
{
    int i, ret, tmp;
    int *storeys;

    argc -= 1; /* 去掉程序名 */
    g_lift_trace_cnt = argc;
    storeys = malloc(MAX_THREAD * argc * sizeof(int));
    if (storeys == NULL) {
        printf("malloc failed!\n");
        return NULL;
    }

    g_lift_trace[0] = storeys;
    g_lift_trace[1] = g_lift_trace[0] + argc;
    g_lift_trace[2] = g_lift_trace[1] + argc;

    for (i = 0; i < argc; i++){
        tmp = str2Digit(argv[i + 1]);
        if (tmp == -1) {
            return NULL;
        }

        g_lift_trace[0][i] = tmp;
        g_lift_trace[1][i] = tmp;
        g_lift_trace[2][i] = tmp;
    }

    return storeys;
}


/* 程序入口 */
int main(int argc, char *argv[])
{
    int i, ret, mode, total = 1;
    int *storeys;
    char *lift_mode;
    LIFT listArgs[MAX_THREAD];
    pid_t child = 0;

    storeys = parse_args(argc, argv);
    ret = init_res();
    if (storeys == NULL || ret != 0) {
        exit(-1);
    }

    /* 环境变量控制运行方式，0/2单线程, 1/3 并发, 4多进程 */
    lift_mode = getenv("LIFT_MODE");
    mode = (lift_mode == NULL) ? 0 : atoi(lift_mode);
    listArgs[0].cnt = argc - 1;
    listArgs[0].mode = mode;
    listArgs[0].storeys = storeys;

    if ((mode % 2) == 1) { /* 多线程模式*/
        for (i = 0; i < MAX_THREAD; i++) {
            pthread_t thread;
            listArgs[i].cnt = argc - 1;
            listArgs[i].mode = mode;
            listArgs[i].storeys = g_lift_trace[i];
            shuffle(argc - 1, g_lift_trace[i]);
            pthread_create(&thread, NULL, lift_entry, &listArgs[i]);
            total++;
        }
    } else if ((mode % 3) == 1) { /* 多进程模式*/
        signal(SIGINT, signal_handler);
        child = fork();
        if (child == 0) {
            printf("********NEW PROC IS %d **********\n", getpid());
            lift_entry(&listArgs[0]);
            /* while 循环等待父进程结束信号 */
            while(!g_child_proc_exit) {
                sleep(1000);
            }

            return 0;
        } else if (child < 0) {
            printf(" fork failed! ");
        }
    }

    lift_entry(&listArgs[0]);
    if (child > 0) {
        kill(child, SIGINT); /* 多进程下，构造父子进程间通信 */
    } else {
        while (g_thread_finish_cnt != total) {
            usleep(10);
        }
    }

    return 0;
}

void print_trace(int index)
{
    int i;

    printf("TRACE[%d] is : \n", index);
    for (i = 0; i < g_lift_trace_cnt; i++) {
        printf("-> %d ", g_lift_trace[index][i]);
    }

    printf("\n");
}

/* 内存保护接口 */
void make_mprotect(void *addr, int len, int prot)
{
    int ret;

    ret = mprotect(addr, len, prot);
    if (ret != 0) {
        printf("mprotect failed, errno : %d\n", errno);
    }
}