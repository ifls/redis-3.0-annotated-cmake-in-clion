/* anet.c -- Basic TCP socket stuff made a bit less boring 主要就是对tcp套接字操作的封装，使它更加符合redis的系统逻辑，同时便于修改
 *
 */

#ifndef ANET_H
#define ANET_H

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

/* Flags used with certain functions. */
#define ANET_NONE 0
#define ANET_IP_ONLY (1<<0)

#if defined(__sun)
#define AF_LOCAL AF_UNIX
#endif

// 创建阻塞 TCP 连接
int anetTcpConnect(char *err, char *addr, int port);

// 创建非阻塞 TCP 连接
int anetTcpNonBlockConnect(char *err, char *addr, int port);

int anetTcpNonBlockBindConnect(char *err, char *addr, int port, char *source_addr);

// 创建阻塞本地Unix连接
int anetUnixConnect(char *err, char *path);

// 创建非阻塞本地Unix连接
int anetUnixNonBlockConnect(char *err, char *path);

// 带 short count 处理的读取函数
int anetRead(int fd, char *buf, int count);

int anetResolve(char *err, char *host, char *ipbuf, size_t ipbuf_len);

int anetResolveIP(char *err, char *host, char *ipbuf, size_t ipbuf_len);

// 创建net服务器, 监听指定tcp端口
int anetTcpServer(char *err, int port, char *bindaddr, int backlog);

// 创建net服务器, 监听指定tcp6端口
int anetTcp6Server(char *err, int port, char *bindaddr, int backlog);

// 创建net服务器, 监听指定unix路径
int anetUnixServer(char *err, char *path, mode_t perm, int backlog);

// tcp 接受新连接
int anetTcpAccept(char *err, int serversock, char *ip, size_t ip_len, int *port);

// unix 接受新连接
int anetUnixAccept(char *err, int serversock);

// 带 short count 处理的写入函数
int anetWrite(int fd, char *buf, int count);

// 将 fd 设置为非阻塞模式
int anetNonBlock(char *err, int fd);

// 禁用 Nagle 算法
int anetEnableTcpNoDelay(char *err, int fd);

// 启用 Nagle 算法
int anetDisableTcpNoDelay(char *err, int fd);

// 开启 TCP 的 keep alive 选项
int anetTcpKeepAlive(char *err, int fd);

// 获取连接客户端的 IP 和端口号
int anetPeerToString(int fd, char *ip, size_t ip_len, int *port);

// 修改 TCP 连接的 keep alive 选项
int anetKeepAlive(char *err, int fd, int interval);

// 获取服务器本机的 IP 和端口号
int anetSockName(int fd, char *ip, size_t ip_len, int *port);

#endif
