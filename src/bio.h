

/* Exported API
 * background I/O Redis 的后台I/O处理
 * */
// 初始化后台任务系统,生成线程
void bioInit(void);

// 创建后台任务
void bioCreateBackgroundJob(int type, void *arg1, void *arg2, void *arg3);

// 返回等待中的 type 类型的工作的数量
unsigned long long bioPendingJobsOfType(int type);

void bioWaitPendingJobsLE(int type, unsigned long long num);

time_t bioOlderJobOfType(int type);

// 不进行清理,直接杀死进程,只在出现严重错误时使用
void bioKillThreads(void);

/* Background job opcodes */
#define REDIS_BIO_CLOSE_FILE    0 /* Deferred close(2) syscall. */
#define REDIS_BIO_AOF_FSYNC     1 /* Deferred AOF fsync. */
#define REDIS_BIO_NUM_OPS       2
