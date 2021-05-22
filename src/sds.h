/* SDSLib, A C dynamic strings library
 */

#ifndef __SDS_H
#define __SDS_H

// simple dynamic string 简单动态字符串
/*
 * 最大预分配长度
 */
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

/*
 * 类型别名,用于指向 sdshdr 的 buf 属性
 */
typedef char *sds;

/*
 * 保存字符串对象的结构
 */
struct sdshdr {

    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];
};

/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void *) (s - (sizeof(struct sdshdr)));
    return sh->len;
}

/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void *) (s - (sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);

sds sdsnew(const char *init);

// 创建一个空字符串
sds sdsempty(void);

// 返回len
size_t sdslen(const sds s);

// 复制字符串
sds sdsdup(const sds s);

// 释放内存
void sdsfree(sds s);

// 返回free
size_t sdsavail(const sds s);

// 将 sds 扩充至指定长度,未使用的空间以 0 字节填充。
sds sdsgrowzero(sds s, size_t len);

// 连接字符串, 指定长度
sds sdscatlen(sds s, const void *t, size_t len);

sds sdscat(sds s, const char *t);

sds sdscatsds(sds s, const sds t);

// 将字符串 t 的前 len 个字符复制到 sds s 当中
sds sdscpylen(sds s, const char *t, size_t len);

sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);

#ifdef __GNUC__

sds sdscatprintf(sds s, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));

#else

sds sdscatprintf(sds s, const char *fmt, ...);

#endif

sds sdscatfmt(sds s, char const *fmt, ...);

// 对 sds 左右两端进行修剪,清除其中 cset 指定的所有字符
sds sdstrim(sds s, const char *cset);

void sdsrange(sds s, int start, int end);

// 未使用, 可能是废弃的
void sdsupdatelen(sds s);

// 保留内存空间, 重置为空字符串
void sdsclear(sds s);

int sdscmp(const sds s1, const sds s2);

// 根据seperate 进行切割, 成多个字符串, count被赋值为数量
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);

void sdsfreesplitres(sds *tokens, int count);

void sdstolower(sds s);

void sdstoupper(sds s);

// 字符串保存整数
sds sdsfromlonglong(long long value);

// 追加 反解义字符串
sds sdscatrepr(sds s, const char *p, size_t len);

// 划分参数
sds *sdssplitargs(const char *line, int *argc);

// replace
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);

// 连接多个字符串, 中间是sep
sds sdsjoin(char **argv, int argc, char *sep);

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);

void sdsIncrLen(sds s, int incr);

sds sdsRemoveFreeSpace(sds s);

size_t sdsAllocSize(sds s);

#endif
