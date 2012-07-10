/*
 * CRT file for libc
 */
#include <stdio.h>
#include <rtthread.h>

void hardware_init_hook(void)
{
}

void software_init_hook(void)
{
}

void *malloc(size_t size)
{
	return rt_malloc(size);
}

void free(void *ptr)
{
	return rt_free(ptr);
}

void *realloc(void *ptr, rt_size_t nbytes)
{
	return rt_realloc(ptr, nbytes);
}

void *calloc(rt_size_t count, rt_size_t size)
{
	return rt_calloc(count, size);
}

char *__ctype_ptr = "__ctype__";
