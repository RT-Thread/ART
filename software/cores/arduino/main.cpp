#include <Arduino.h>

extern "C"{
int __main(void);
}

void (*init_end[1])(void)
  __attribute__((section(".init_array"))) = { (void (*)())0 };

extern void (*init_start[1])(void);

static void construct(void)
{
	rt_uint32_t* call;

	for(call = (rt_uint32_t*)&init_start + 1; call < (rt_uint32_t*)&init_end; call++)
	{
		((void (*)(void))*call)();
	}
}

int __main(void)
{
	construct();

	init();

	setup();

	for (;;) {
	  loop();
	}

	return 0;
}

/* mark as a weak main function implementation */
int main() __attribute__ ((weak,alias("__main")));
