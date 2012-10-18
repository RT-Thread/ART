#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

// <RDTConfigurator URL="http://www.rt-thread.com/eclipse">

// <integer name="RT_NAME_MAX" description="Maximal size of kernel object name length" default="6" />
#define RT_NAME_MAX	6
// <integer name="RT_ALIGN_SIZE" description="Alignment size for CPU architecture data access" default="4" />
#define RT_ALIGN_SIZE	4
// <integer name="RT_THREAD_PRIORITY_MAX" description="Maximal level of thread priority" default="2">
// <item description="8">8</item>
// <item description="32">32</item>
// <item description="256">256</item>
// </integer>
#define RT_THREAD_PRIORITY_MAX	32
// <integer name="RT_TICK_PER_SECOND" description="OS tick per second" default="100" />
#define RT_TICK_PER_SECOND	100
// <integer name="IDLE_THREAD_STACK_SIZE" description="The stack size of idle thread" default="512" />
#define IDLE_THREAD_STACK_SIZE	512
// <bool name="RT_USING_MODULE" description="Using Application Module" default="true" />
// #define RT_USING_MODULE
// <section name="RT_DEBUG" description="Kernel Debug Configuration" default="true" >
#define RT_DEBUG
// <bool name="RT_THREAD_DEBUG" description="Thread debug enable" default="false" />
// #define RT_THREAD_DEBUG
// <bool name="RT_USING_OVERFLOW_CHECK" description="Thread stack over flow detect" default="true" />
#define RT_USING_OVERFLOW_CHECK
// </section>

// <bool name="RT_USING_HOOK" description="Using hook functions" default="true" />
#define RT_USING_HOOK
// <section name="IPC" description="Inter-Thread communication" default="always" >
// <bool name="RT_USING_SEMAPHORE" description="Using semaphore in the system" default="true" />
#define RT_USING_SEMAPHORE
// <bool name="RT_USING_MUTEX" description="Using mutex in the system" default="true" />
#define RT_USING_MUTEX
// <bool name="RT_USING_EVENT" description="Using event group in the system" default="true" />
#define RT_USING_EVENT
// <bool name="RT_USING_MAILBOX" description="Using mailbox in the system" default="true" />
#define RT_USING_MAILBOX
// <bool name="RT_USING_MESSAGEQUEUE" description="Using message queue in the system" default="true" />
#define RT_USING_MESSAGEQUEUE
// </section>

// <section name="MM" description="Memory Management" default="always" >
// <bool name="RT_USING_MEMPOOL" description="Using Memory Pool Management in the system" default="true" />
#define RT_USING_MEMPOOL
// <bool name="RT_USING_HEAP" description="Using Dynamic Heap Management in the system" default="true" />
#define RT_USING_HEAP
// <bool name="RT_USING_MEMHEAP" description="Using extra-dynamic heap management in the system" default="true" />
#define RT_USING_MEMHEAP
// <bool name="RT_USING_SMALL_MEM" description="Optimizing for small memory" default="false" />
#define RT_USING_SMALL_MEM
// <bool name="RT_USING_SLAB" description="Using SLAB memory management for large memory" default="false" />
// #define RT_USING_SLAB
// </section>

// <section name="RT_USING_DEVICE" description="Using Device Driver Framework" default="true" >
#define RT_USING_DEVICE
// <bool name=RT_USING_DEVICE_IPC description="Using IPC in Device Driver Framework" default="true" />
#define RT_USING_DEVICE_IPC
// <bool name="RT_USING_SERIAL" description="Using Serial Device Driver Framework" default="true" />
#define RT_USING_SERIAL
// <integer name="RT_UART_RX_BUFFER_SIZE" description="The buffer size for UART reception" default="64" />
#define RT_UART_RX_BUFFER_SIZE	64
// </section>

// <section name="RT_USING_USB_DEVICE" description="USB Device stack" default="true" >
#define RT_USING_USB_DEVICE
// </section>
// <section name="RT_USING_CONSOLE" description="Using console" default="true" >
#define RT_USING_CONSOLE
// <integer name="RT_CONSOLEBUF_SIZE" description="The buffer size for console output" default="128" />
#define RT_CONSOLEBUF_SIZE	128
// <string name="RT_CONSOLE_DEVICE_NAME" description="The device name for console output" default="vcom">
// <item name="USB virtual serial">"vcom"</item>
// <item name="UART1">"uart1"</item>
// </string>
#define RT_CONSOLE_DEVICE_NAME "vcom"
// </section>

// <bool name="RT_USING_COMPONENTS_INIT" description="Using components manager" default="true" />
#define RT_USING_COMPONENTS_INIT

// <section name="RT_USING_FINSH" description="Using finsh as shell, which is a C-Express shell" default="true" >
#define RT_USING_FINSH
// <bool name="FINSH_USING_SYMTAB" description="Using symbol table in finsh shell" default="true" />
#define FINSH_USING_SYMTAB
// <bool name="FINSH_USING_DESCRIPTION" description="Keeping description in symbol table" default="true" />
#define FINSH_USING_DESCRIPTION
// <integer name="FINSH_THREAD_STACK_SIZE" description="The stack size for finsh thread" default="4096" />
#define FINSH_THREAD_STACK_SIZE	4096
// </section>

// <section name="LIBC" description="C Runtime library setting" default="always" >
// <bool name="RT_USING_NEWLIB" description="Using newlib library, only available under GNU GCC" default="true" />
// #define RT_USING_NEWLIB
// <bool name="RT_USING_PTHREADS" description="Using POSIX threads library" default="true" />
// #define RT_USING_PTHREADS
// </section>

// </RDTConfigurator>

#endif
