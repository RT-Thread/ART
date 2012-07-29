#include <rtthread.h>

int a = 0x10;
int b = 0x1e0;
int c = 0x5ff;

int main(void)
{
	a = a + 1;
	b = b + 1;
	c = c + 1;
	rt_kprintf("hello world\n");

	return 0;
}
