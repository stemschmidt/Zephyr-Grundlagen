#include <zephyr/device.h>
#include <zephyr/drivers/led.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

#define NUM_LEDS 4

typedef enum
{
    timer = 0,
    delay_plus,
    delay_minus
} nachrichten_t;

K_MSGQ_DEFINE(message_queue, sizeof(nachrichten_t), 10, 4);

static void led_timer_callback(struct k_timer *dummy);
static void key_press(struct input_event *evt, void *user_data);

K_TIMER_DEFINE(led_timer, led_timer_callback, NULL);
INPUT_CALLBACK_DEFINE(NULL, key_press, NULL);

const struct device *leds = DEVICE_DT_GET(DT_PATH(leds));

static void update_led(void)
{
    static int current = 0;

    /* Alle LEDs ausschalten */
    for (int i = 0; i < NUM_LEDS; i++)
    {
        led_off(leds, i);
    }

    /* Aktuelle LED einschalten */
    led_on(leds, current);

    /* Nächste LED */
    current = (current + 1) % NUM_LEDS;
}

int main(void)
{
    int delay_ms = 500;

    if (!device_is_ready(leds))
    {
        return -1;
    }

    k_timer_start(&led_timer, K_MSEC(delay_ms), K_NO_WAIT);

    while (1)
    {
        nachrichten_t nachricht;
        k_msgq_get(&message_queue, &nachricht, K_FOREVER);

        switch (nachricht)
        {
            case timer:
                printk("start timer with = %d\n", delay_ms);
                k_timer_start(&led_timer, K_MSEC(delay_ms), K_NO_WAIT);
                update_led();
                break;
            case delay_plus:
                if (delay_ms < 1000)
                {
                    delay_ms += 100;
                    printk("delay_ms = %d\n", delay_ms);
                }
                break;
            case delay_minus:
                if (delay_ms > 100)
                {
                    delay_ms -= 100;
                    printk("delay_ms = %d\n", delay_ms);
                }
                break;
            default:
                printk("unbekannte nachricht\n");
                break;
        }
    }

    return 0;
}

static void led_timer_callback(struct k_timer *dummy)
{
    const nachrichten_t update = timer;
    int err = k_msgq_put(&message_queue, &update, K_NO_WAIT);
    if (err != 0)
    {
        printk("konnte timer nicht in queue schreiben (%d)\n", err);
    }
}

static void key_press(struct input_event *evt, void *user_data)
{
    int err = 0;

    switch (evt->code)
    {
        case INPUT_KEY_1:
            if (evt->value == 1)
            {
                const nachrichten_t update = delay_minus;
                err = k_msgq_put(&message_queue, &update, K_NO_WAIT);
                if (err != 0)
                {
                    printk("konnte delay_minus nicht in queue schreiben (%d)\n", err);
                }
            }
            else
            {
                printk("Button1 inaktiv\n");
            }
            break;
        case INPUT_KEY_3:
            if (evt->value == 1)
            {
                const nachrichten_t update = delay_plus;
                err = k_msgq_put(&message_queue, &update, K_NO_WAIT);
                if (err != 0)
                {
                    printk("konnte delay_plus nicht in queue schreiben (%d)\n", err);
                }
            }
            else
            {
                printk("Button3 inaktiv\n");
            }
            break;
        default:
            printk("unbekannter Button\n");
            break;
    }
}
