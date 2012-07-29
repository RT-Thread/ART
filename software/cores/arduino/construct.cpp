#include <Arduino.h>

extern void (*init_start[1])(void);
extern void (*init_end[1])(void);

void construct(void)
{
	rt_uint32_t* call;

	for(call = (rt_uint32_t*)&init_start + 1; call < (rt_uint32_t*)&init_end; call++)
	{
		((void (*)(void))*call)();
	}
}