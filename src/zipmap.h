/* String -> String Map data structure optimized for size.
 *
 * See zipmap.c for more info.
 */

#ifndef _ZIPMAP_H
#define _ZIPMAP_H

// 注意, 从 2.6 版本开始, Redis 使用 ziplist 来表示小Hash, 而不再使用zipmap,
// 为节约空间而实现的字符串到字符串映射结构
unsigned char *zipmapNew(void);

unsigned char *
zipmapSet(unsigned char *zm, unsigned char *key, unsigned int klen, unsigned char *val, unsigned int vlen, int *update);

unsigned char *zipmapDel(unsigned char *zm, unsigned char *key, unsigned int klen, int *deleted);

unsigned char *zipmapRewind(unsigned char *zm);

unsigned char *
zipmapNext(unsigned char *zm, unsigned char **key, unsigned int *klen, unsigned char **value, unsigned int *vlen);

int zipmapGet(unsigned char *zm, unsigned char *key, unsigned int klen, unsigned char **value, unsigned int *vlen);

int zipmapExists(unsigned char *zm, unsigned char *key, unsigned int klen);

unsigned int zipmapLen(unsigned char *zm);

size_t zipmapBlobLen(unsigned char *zm);

void zipmapRepr(unsigned char *p);

#endif
