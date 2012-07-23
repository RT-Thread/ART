#include <rtthread.h>

void my_thread(void* parameter)
{
  while (1)
  {
     rt_kprintf("thread: %s\n", rt_thread_self()->name);
     rt_thread_delay(2 * RT_TICK_PER_SECOND);
  }
}

int main(void)
{
  rt_thread_t tid;
  rt_kprintf("Hello World\n");

  tid = rt_thread_create("mythread", my_thread, RT_NULL,
     1024, 20, 5);
  if (tid != RT_NULL) rt_thread_startup(tid);

  while (1)
  {
     rt_thread_delay(RT_TICK_PER_SECOND);
     rt_kprintf("thread: %s\n", rt_thread_self()->name);
  }
  return 0;
}
