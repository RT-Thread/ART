/* the stub code for libc when using minilibc */
#include <stdio.h>
#include <rtthread.h>

#ifdef RT_USING_MINILIBC
void
_exit (int status)
{
	rt_kprintf("thread:%s exit with %d\n", rt_thread_self()->name, status);
	RT_ASSERT(0);

	while (1);
}

int
_kill(int pid, int sig)
{
	return -1;
}

int
_getpid(void)
{
	return 0;
}
#endif
