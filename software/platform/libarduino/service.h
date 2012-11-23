/*
 * Kernel Service Thread
 *
 */
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SERVICE_PRIORITY    15
#define SERVICE_STACK_SIZE  2048

typedef void (*service_func_t)(void* arg0, void* arg1, void* arg2, void* arg3);

rt_err_t service_do(service_func_t service, void* arg0, void* arg1, void* arg2, void* arg3);
rt_err_t service_do_async(service_func_t service, void* arg0, void* arg1, void* arg2, void* arg3);

void service_init(void);

#ifdef __cplusplus
}
#endif
