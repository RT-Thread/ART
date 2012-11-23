#include "service.h"
#include <rtdevice.h>

struct service_item
{
    service_func_t func;

    void* arg0;
    void* arg1;
    void* arg2;
    void* arg3;

    struct rt_completion *ack;
};

static struct rt_messagequeue* _service_mq;

rt_err_t service_do(service_func_t service, void* arg0, void* arg1, void* arg2, void* arg3)
{
    rt_err_t result;
    struct service_item item;
    struct rt_completion ack;

    /* initialize ack */
    rt_completion_init(&ack);

    item.func = service;
    item.arg0 = arg0;
    item.arg1 = arg1;
    item.arg2 = arg2;
    item.arg3 = arg3;
    item.ack  = &ack;

    /* send to */
    result = rt_mq_send(_service_mq, &item, sizeof(item));
    if (result == RT_EOK)
    {
        /* wait for completion */
        result = rt_completion_wait(&ack, RT_WAITING_FOREVER);
    }

    return result;
}

rt_err_t service_do_async(service_func_t service, void* arg0, void* arg1, void* arg2, void* arg3)
{
    rt_err_t result;
    struct service_item item;

    item.func = service;
    item.arg0 = arg0;
    item.arg1 = arg1;
    item.arg2 = arg2;
    item.arg3 = arg3;
    item.ack  = RT_NULL;

    /* send to */
    result = rt_mq_send(_service_mq, &item, sizeof(item));

    return result;
}

static void _service_entry(void* parameter)
{
    rt_err_t result;
    struct service_item item;

    while (1)
    {
        result = rt_mq_recv(_service_mq, &item, sizeof(struct service_item), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* do the service */
            item.func(item.arg0, item.arg1, item.arg2, item.arg3);

            /* acknowledge */
            if (item.ack != RT_NULL)
            {
                rt_completion_done(item.ack);
            }
        }
    }
}

void service_init(void)
{
    rt_thread_t tid;

    _service_mq = rt_mq_create("ksrv", sizeof(struct service_item), 4, RT_IPC_FLAG_FIFO);
    if (_service_mq == RT_NULL)
    {
        rt_kprintf("out of memory.\n");
        return;
    }

    tid = rt_thread_create("ksrv", _service_entry, RT_NULL,
        SERVICE_STACK_SIZE, SERVICE_PRIORITY, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
}
