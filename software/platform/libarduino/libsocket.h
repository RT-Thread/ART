#ifndef __LIB_SOCKET_H__
#define __LIB_SOCKET_H__

#include <rtthread.h>
#include <inttypes.h>

#ifdef RT_USING_LWIP

#ifdef __cplusplus
extern "C" {
#endif

/* TCP */
#ifndef SOCK_STREAM
#define SOCK_STREAM     1
#endif
/* UDP */
#ifndef SOCK_DGRAM
#define SOCK_DGRAM      2
#endif

int socket_create(int type);
int socket_connect(int sock, const char* host, int port);
int socket_connect_addr(int sock, uint32_t addr, int port);
int socket_read(int sock, void* buf, size_t bufsz);
int socket_write(int sock, const void* buf, size_t length);
int socket_close(int sock);
int socket_printf(int sock, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif

#endif
