/* redisassert.h -- Drop in replacemnet(它是指能够在不要求任何其他代码或配置更改且不会造成负面影响的情况下，
 * 用另一种硬件（或软件）组件替换另一种硬件（或软件）组件的能力。) assert.h that prints the stack trace
 *                  in the Redis logs.
 *                  代替 libc里的 assert函数
 */

#ifndef __REDIS_ASSERT_H__
#define __REDIS_ASSERT_H__

#include <unistd.h> /* for _exit() */

#define assert(_e) ((_e)?(void)0 : (_redisAssert(#_e,__FILE__,__LINE__),_exit(1)))

void _redisAssert(char *estr, char *file, int line);

#endif
