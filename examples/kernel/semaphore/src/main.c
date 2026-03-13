#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define PRIORITY 5

static int sensor_value = 0;

static void producer_device_callback(struct k_timer *dummy);
static void device_work_handler(struct k_work *work);

// Synchronisation von Interrupt (über WorkQueue) zu Thread
K_SEM_DEFINE(data_ready, 0, 1);

// Timer callback wird im Interrupt-Kontext aufgerufen
K_TIMER_DEFINE(producer_device, producer_device_callback, NULL);
// Workqueue printk (nicht im Interrupt-Kontext aufrufen!)
K_WORK_DEFINE(device_work, device_work_handler);

void device_work_handler(struct k_work *work)
{
    printk("Erzeuger: Neuer Wert %d\n", sensor_value);
    // Wecke Konsumenten
    k_sem_give(&data_ready);
}

static void producer_device_callback(struct k_timer *dummy)
{
    // Neuer Sensorwert!
    sensor_value++;
    k_work_submit(&device_work);
}

int main(void)
{
    k_timer_start(&producer_device, K_MSEC(1000), K_MSEC(1000));
    while (1)
    {
        // Warte auf Daten vom Produzenten
        k_sem_take(&data_ready, K_FOREVER);

        printk("Konsument: Verarbeite Wert %d\n", sensor_value);
    }

    return 0;
}
