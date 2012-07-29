#include <Arduino.h>
#include <rtthread.h>

class CTest
{
public:
	CTest(int a, int b);
	int a;
	int b;
};

CTest::CTest(int a, int b)
	: a(a), b(b)
{
}

CTest t(10, 100);

int main(void)
{
	construct();

	rt_kprintf("t.a = %d, t.b = %d\n", t.a, t.b);
}
