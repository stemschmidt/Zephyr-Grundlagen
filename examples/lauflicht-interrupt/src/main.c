#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

#define NUM_LEDS 4

#define SW1_NODE DT_ALIAS(sw1) // BUTTON 1
#define SW3_NODE DT_ALIAS(sw3) // BUTTON 3

/* Kompilezeit-Fehler mit eindeutiger Fehlerbeschreibung */
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "sw1 alias not defined"
#endif

#if !DT_NODE_HAS_STATUS(SW3_NODE, okay)
#error "sw3 alias not defined"
#endif

typedef enum
{
    timer = 0,
    delay_plus,
    delay_minus
} nachrichten_t;

K_MSGQ_DEFINE(message_queue, sizeof(nachrichten_t), 10, 4);

static void led_timer_callback(struct k_timer *dummy);

K_TIMER_DEFINE(led_timer, led_timer_callback, NULL);

static const struct device *leds = DEVICE_DT_GET(DT_PATH(leds));
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);

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

/* Callbacks */
static struct gpio_callback button1_cb_data;
static struct gpio_callback button3_cb_data;

/* ISR Callbacks */
static void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    const nachrichten_t update = delay_minus;
    k_msgq_put(&message_queue, &update, K_NO_WAIT);
}

static void button3_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    const nachrichten_t update = delay_plus;
    k_msgq_put(&message_queue, &update, K_NO_WAIT);
}

static int init_button(void)
{
    /* BUTTON 1 konfigurieren */
    if (!device_is_ready(button1.port))
    {
        printk("Button1 device not ready\n");
        return 0;
    }

    int ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
    if (ret < 0)
    {
        printk("gpio_pin_configure_dt für Button1: %d\n", ret);
        return -1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0)
    {
        printk("gpio_pin_interrupt_configure_dt für Button1: %d\n", ret);
        return -1;
    }

    gpio_init_callback(&button1_cb_data, button1_pressed, BIT(button1.pin));

    ret = gpio_add_callback(button1.port, &button1_cb_data);
    if (ret < 0)
    {
        printk("gpio_add_callback für Button1: %d\n", ret);
        return -1;
    }

    /* BUTTON 3 konfigurieren */
    if (!device_is_ready(button3.port))
    {
        printk("Button3 device not ready\n");
        return -1;
    }

    ret = gpio_pin_configure_dt(&button3, GPIO_INPUT);
    if (ret < 0)
    {
        printk("gpio_pin_configure_dt für Button3: %d\n", ret);
        return -1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button3, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0)
    {
        printk("gpio_pin_interrupt_configure_dt für Button3: %d\n", ret);
        return -1;
    }

    gpio_init_callback(&button3_cb_data, button3_pressed, BIT(button3.pin));

    ret = gpio_add_callback(button3.port, &button3_cb_data);
    if (ret < 0)
    {
        printk("gpio_add_callback für Button3: %d\n", ret);
        return -1;
    }

    return 0;
}

int main(void)
{
    int delay_ms = 500;

    if (!device_is_ready(leds))
    {
        return -1;
    }

    if (init_button() != 0)
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
                printk("Unbekannte Nachricht\n");
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
