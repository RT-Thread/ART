#include <lwip/netdb.h>
#include <lwip/sockets.h>

#include "libsocket.h"

#ifdef RT_USING_LWIP
int socket_create(int type)
{
    int sock = lwip_socket(AF_INET, type, 0);
    return sock;
}
RTM_EXPORT(socket_create);

int socket_connect(int sock, const char* host, int port)
{
    int result;
    int timeout;
    struct hostent *server_host;
    struct sockaddr_in server_addr;

    server_host = gethostbyname(host);
    if (server_host == RT_NULL) 
    {
        rt_kprintf("unknow host: %s\n", host);
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)server_host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    result = lwip_connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    /* set recv timeout */
    timeout = 3000; /* 3 seconds */
    lwip_setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    return result;
}
RTM_EXPORT(socket_connect);

int socket_connect_addr(int sock, uint32_t addr, int port)
{
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = addr;
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    return lwip_connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
}
RTM_EXPORT(socket_connect_addr);

int socket_read(int sock, void* buf, size_t bufsz)
{
    int result;

    result = lwip_read(sock, buf, bufsz);
    return result;
}
RTM_EXPORT(socket_read);

int socket_write(int sock, const void* buf, size_t length)
{
    return lwip_write(sock, buf, length);
}
RTM_EXPORT(socket_write);

int socket_close(int sock)
{
    return lwip_close(sock);
}
RTM_EXPORT(socket_close);

int socket_printf(int sock, const char* fmt, ...)
{
    va_list args;
    rt_uint32_t length;
    rt_uint8_t *buf_ptr;

#define BUF_SZ  1024
    buf_ptr = (rt_uint8_t*) rt_malloc (1024);

    va_start(args, fmt);
    length = vsnprintf((char*)(buf_ptr), BUF_SZ, fmt, args);
    va_end(args);

    lwip_send(sock, buf_ptr, length, 0);

    rt_free(buf_ptr);

    return length;
}
RTM_EXPORT(socket_printf);

#endif
