#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/*
  pc contains the address where the issue occured. Use
  "/opt/toolchains/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-addr2line
    -e build/lauflicht-threads/zephyr/zephyr.elf 0x12345678" (address in pc)
  to find the violating function.
*/

void k_sys_fatal_error_handler(unsigned int reason, const struct arch_esf *esf)
{
    struct k_thread *thread = k_current_get();

    // Thread Infos
    printk("Thread: %p\n", thread);
    printk("Name: %s\n", k_thread_name_get(thread));

    __asm__ volatile("bkpt #0");

    while (1)
    {
        k_cpu_idle();
    }
}
