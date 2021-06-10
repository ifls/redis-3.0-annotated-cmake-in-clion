/* A simple event-driven programming library. Originally I wrote this code 事件处理器实现（基于 Reactor 模式）。
 * for the Jim's event-loop (Jim is a Tcl interpreter) but later translated
 * it in form of a library for easy reuse.
 */

#ifndef __AE_H__
#define __AE_H__

/*
 * 事件执行状态
 */
// 成功
#define AE_OK 0
// 出错
#define AE_ERR -1

/*
 * 文件事件状态
 */
// 未设置
#define AE_NONE 0
// 可读
#define AE_READABLE 1
// 可写
#define AE_WRITABLE 2

/*
 * 时间处理器的执行 flags
 */
// 文件事件
#define AE_FILE_EVENTS 1
// 时间事件
#define AE_TIME_EVENTS 2
// 所有事件
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
// 不阻塞,也不进行等待
#define AE_DONT_WAIT 4

/*
 * 决定时间事件是否要持续执行的 flag
 */
#define AE_NOMORE -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

/*
 * 事件处理器状态
 */
struct aeEventLoop;

/* Types and data structures 
 *
 * 事件接口
 * 设置事件触发时的处理函数原型
 */
typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);

typedef int aeTimeProc(struct aeEventLoop *eventLoop, long long id, void *clientData);

// 时间事件 析构函数
typedef void aeEventFinalizerProc(struct aeEventLoop *eventLoop, void *clientData);

typedef void aeBeforeSleepProc(struct aeEventLoop *eventLoop);

/* File event structure
 *
 * 文件/网络io socket 事件结构
 */
typedef struct aeFileEvent {

    // 监听事件类型掩码,
    // 值可以是 AE_READABLE 或 AE_WRITABLE,
    // 或者 AE_READABLE | AE_WRITABLE
    int mask; /* one of AE_(READABLE|WRITABLE) */

    // 读事件处理器
    aeFileProc *rfileProc;

    // 写事件处理器
    aeFileProc *wfileProc;

    // 多路复用库的私有数据
    void *clientData;  // 就是 redisClient* client

} aeFileEvent;

/* Time event structure
 *
 * 时间事件结构
 */
typedef struct aeTimeEvent {

    // 时间事件的唯一标识符
    long long id; /* time event identifier. */

    // 事件的到达时间, 两个时间是要加起来的
    long when_sec; /* seconds */
    long when_ms; /* milliseconds */

    // 事件处理函数
    aeTimeProc *timeProc;

    // 事件释放函数
    aeEventFinalizerProc *finalizerProc;

    // 多路复用库的私有数据
    void *clientData;

    // 指向下个时间事件结构,形成链表
    struct aeTimeEvent *next;

} aeTimeEvent;

/* A fired event
 *
 * 已就绪事件
 */
typedef struct aeFiredEvent {

    // 已就绪文件描述符
    int fd;

    // 事件类型掩码,
    // 值可以是 AE_READABLE 或 AE_WRITABLE
    // 或者是两者的或
    int mask;

} aeFiredEvent;

/* State of an event based program 
 *
 * 事件处理器的状态
 */
typedef struct aeEventLoop {

    // 目前已注册的最大描述符
    int maxfd;   /* highest file descriptor currently registered */

    // 文件描述符集合 最大大小, 用作epoll的最大事件
    int setsize; /* max number of file descriptors tracked */

    // 用于 生成时间事件 id
    long long timeEventNextId;

    // 上一次执行时间事件的时间
    time_t lastTime;     /* Used to detect system clock skew */

    // 已注册的文件事件
    aeFileEvent *events; /* 数组, 传到epoll里面 Registered events */

    // 已就绪的文件事件
    aeFiredEvent *fired; /* 数组 Fired events */

    // 时间事件
    aeTimeEvent *timeEventHead;

    // 事件处理器的开关
    int stop;

    // 多路复用库的 特有数据
    void *apidata; /* This is used for polling API specific data */

    // 在处理事件前要执行的函数
    aeBeforeSleepProc *beforesleep;  //

} aeEventLoop;

/* Prototypes */
// 创建事件循环
aeEventLoop *aeCreateEventLoop(int setsize);

// 删除事件循环
void aeDeleteEventLoop(aeEventLoop *eventLoop);

// 停止事件处理
void aeStop(aeEventLoop *eventLoop);

// 创建文件事件
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask, aeFileProc *proc, void *clientData);

// 删除文件事件
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);

// 获取文件事件
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);

// 创建时间事件
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds, aeTimeProc *proc, void *clientData,
                            aeEventFinalizerProc *finalizerProc);

// 删除时间事件
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);

// 处理所有已到达的时间事件,以及所有已就绪的文件事件.
int aeProcessEvents(aeEventLoop *eventLoop, int flags);

// 在给定毫秒内等待,直到 fd 变成可写、可读或异常
int aeWait(int fd, int mask, long long milliseconds);

// 事件处理总函数, 阻塞
void aeMain(aeEventLoop *eventLoop);

// 返回当前使用的多路复用组件的名字
char *aeGetApiName(void);

// 设置处理事件前需要被执行的函数
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep);

// 返回当前事件槽大小
int aeGetSetSize(aeEventLoop *eventLoop);

// 调整事件槽的大小
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize);

#endif
