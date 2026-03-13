#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// #define USE_BUSY_SLEEP
// #define USE_MUTEX

#define STACK_SIZE 1024
#define COOPERATIVE_PRIO -5
#define PREEMPTIVE_PRIO 5
#define WORKER1_PRIORITY PREEMPTIVE_PRIO

K_THREAD_STACK_DEFINE(worker1_stack_area, STACK_SIZE);
K_MUTEX_DEFINE(uart_mutex);

static void custom_sleep(int ms)
{
#ifdef USE_BUSY_SLEEP
    int64_t reftime = k_uptime_get();
    int64_t diff = 0;
    do
    {
        diff = k_uptime_get() - reftime;
    } while (diff < (int64_t) ms);
#else
    k_sleep(K_MSEC(ms));
#endif
}

void worker1(void *arg1, void *arg2, void *arg3)
{
    while (1)
    {
#ifdef USE_MUTEX
        k_mutex_lock(&uart_mutex, K_FOREVER);
#endif
        printk("worker1: start ");
        custom_sleep(500);
        printk("and end critical section\n");
#ifdef USE_MUTEX
        k_mutex_unlock(&uart_mutex);
#endif
        custom_sleep(1000);
    }
}

int main(void)
{
    struct k_thread worker1_thread_data;

    k_tid_t tid = k_thread_create(&worker1_thread_data,
        worker1_stack_area,
        K_THREAD_STACK_SIZEOF(worker1_stack_area),
        worker1,
        NULL,
        NULL,
        NULL,
        WORKER1_PRIORITY,
        0,
        K_NO_WAIT);
    k_thread_name_set(tid, "worker1_id");

    while (1)
    {
#ifdef USE_MUTEX
        k_mutex_lock(&uart_mutex, K_FOREVER);
#endif
        printk("main: start ");
        custom_sleep(200);
        printk("and end critical section\n");
#ifdef USE_MUTEX
        k_mutex_unlock(&uart_mutex);
#endif
        custom_sleep(1000);
    }
}
