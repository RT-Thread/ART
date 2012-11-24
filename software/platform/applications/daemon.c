/*
 * File      : daemon.c
 * This file is part of RT-Thread ART.
 * COPYRIGHT (C) 2012, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-11-24     Bernard      the first version
 */

#include <rtthread.h>

#if defined(RT_USING_LWIP) && defined(RT_USING_DFS)

#include <dfs_posix.h>
#include <lwip/sockets.h>

#define SESSION_MAX	5
#define BUF_SZ		1024

#define FILE_PATH   "/sd"

struct client_request
{
	int socket;

	rt_uint32_t filesz;
	rt_uint8_t  filename[64];
};

static rt_uint8_t client_session = 0;
void client_thread(void* parameter)
{
	int fd;
	rt_uint32_t level;
	rt_uint32_t recv_length;
	rt_uint8_t *buf_ptr = RT_NULL;
	struct client_request* request;
	char fn_full[64];

	request = (struct client_request*) parameter;
	RT_ASSERT(request != RT_NULL);

	buf_ptr = (rt_uint8_t*) rt_malloc (BUF_SZ);
	if (buf_ptr == RT_NULL) goto __exit;

	rt_snprintf(fn_full, sizeof(fn_full), "%s/%s", FILE_PATH, request->filename);
	rt_kprintf("\nWrite to file: %s\n", fn_full);

	fd = open(fn_full, O_RDWR | O_TRUNC, 0);
	if (fd >= 0)
	{
		while (1)
		{
			recv_length = lwip_recv(request->socket, buf_ptr, BUF_SZ, 0);
			if (recv_length <= 0)
			{
				break;
			}

			rt_kprintf("<", recv_length);
			write(fd, buf_ptr, recv_length);
		}
	}

__exit:
	if (buf_ptr != RT_NULL) 
		rt_free(buf_ptr);
	if (request != RT_NULL)
		rt_free(request);
	if (fd >= 0)
	{
		rt_kprintf("\nDone.\n");
		close(fd);
	}
	
	level = rt_hw_interrupt_disable();
	client_session --;
	rt_hw_interrupt_enable(level);

	return;
}

void daemon_thread(void* parameter)
{
	int timeout;
	int sock, client;
	rt_uint32_t level;
	rt_uint32_t sin_size;
	struct sockaddr_in server_addr, client_addr;

	sin_size = sizeof(struct sockaddr_in);

	/* create server socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	   rt_kprintf("Socket error\n");
	   return;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	rt_memset(&(server_addr.sin_zero),8, sizeof(server_addr.sin_zero));

	if (lwip_bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
	   rt_kprintf("Unable to bind\n");
	   lwip_close(sock);
	   return;
	}

	if (lwip_listen(sock, 5) == -1)
	{
	   rt_kprintf("Listen error\n");
	   lwip_close(sock);
	   return;
	}

	while(1)
	{
		client = lwip_accept(sock, (struct sockaddr *)&client_addr, &sin_size);
		if (client >= 0)
		{
			struct client_request* request;

			level = rt_hw_interrupt_disable();
			if (client_session >= SESSION_MAX)
			{
				rt_hw_interrupt_enable(level);
				continue;
			}
			rt_hw_interrupt_enable(level);

			request = (struct client_request*) rt_malloc (sizeof(struct client_request));
			if (request != RT_NULL)
			{
				rt_thread_t tid;

				/* set recv timeout */
				timeout = 3000; /* 3 seconds */				
				lwip_setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
				if (lwip_recv(client, request, sizeof(struct client_request), 0) != sizeof(struct client_request))
				{
					 lwip_close(client);
					 rt_free(request);
				}
				else
				{
					/* set client socket */
					request->socket = client;
					tid = rt_thread_create("client", client_thread, (void*) request, 
						1024, 30, 10);
					if (tid != RT_NULL)
					{

						level = rt_hw_interrupt_disable();
						client_session ++;
						rt_hw_interrupt_enable(level);
						rt_thread_startup(tid);
					}
					else
					{
						/* create thread failed. */
						lwip_close(client);
						rt_free(request);
					}
				}			
			}
		}
	}

	lwip_close(sock);

	return ;
}

int daemon(void)
{
	rt_thread_t tid;

	tid = rt_thread_create("daemon", daemon_thread, RT_NULL,
		1024, 20, 10);
	if (tid != RT_NULL)
		rt_thread_startup(tid);

	return 0;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(daemon, startup daemon server);
#endif

#endif
