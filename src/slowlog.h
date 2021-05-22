#define SLOWLOG_ENTRY_MAX_ARGC 32
#define SLOWLOG_ENTRY_MAX_STRING 128

/* This structure defines an entry inside the slow log list */
/*
 * 慢查询日志
 */
typedef struct slowlogEntry {

    // 命令与命令参数
    robj **argv;

    // 命令与命令参数的数量
    int argc;

    // 唯一标识符
    long long id;       /* Unique entry identifier. */

    // 执行命令消耗的时间,以微秒为单位
    // 注释里说的 nanoseconds 是错误的
    long long duration; /* Time spent by the query, in nanoseconds. */

    // 命令执行时的时间,格式为 UNIX 时间戳
    time_t time;        /* Unix time at which the query was executed. */

} slowlogEntry;

/* Exported API */
void slowlogInit(void);

void slowlogPushEntryIfNeeded(robj **argv, int argc, long long duration);

/* Exported commands */
void slowlogCommand(redisClient *c);
