#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>

#define NUM_LEDS 4

static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(DT_NODELABEL(button3), gpios);

const struct device *leds = DEVICE_DT_GET(DT_NODELABEL(leds));
// const struct device *leds = DEVICE_DT_GET(DT_PATH(leds));

static void button_auswerten(int64_t current_ms)
{
    static int64_t last_time = 0;

    if (current_ms - last_time >= 10)
    {
        last_time = current_ms;
        /* Buttons abfragen */
        int b1 = gpio_pin_get_dt(&button1); // gedrückt (active-low)
        int b3 = gpio_pin_get_dt(&button3); // gedrückt (active-low)

        if (b1 != 0)
        {
            printk("Button1!\n");
        }

        if (b3 != 0)
        {
            printk("Button3!\n");
        }
    }
}

static void led_ansteuern(int64_t current_ms)
{
    static int64_t last_time = 0;
    static int current = 0;

    if (current_ms - last_time >= 1000)
    {
        //        printk("led delta: %lld\n", current_ms - last_time);
        last_time = current_ms;
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
}

static void aufgaben_erledigen(void)
{
    uint8_t rand = 0;
    do
    {
        rand = sys_rand8_get() % 10;
    } while (rand == 0);
    k_sleep(K_MSEC(rand));
}

int main(void)
{
    int64_t milliseconds = 0;

    if (!device_is_ready(leds))
    {
        return -1;
    }

    /* Buttons konfigurieren */
    if (!device_is_ready(button1.port) || !device_is_ready(button3.port))
    {
        return -1;
    }

    gpio_pin_configure_dt(&button1, GPIO_INPUT);
    gpio_pin_configure_dt(&button3, GPIO_INPUT);

    while (1)
    {
        milliseconds = k_uptime_get();

        led_ansteuern(milliseconds);
        button_auswerten(milliseconds);
        //        aufgaben_erledigen();
    }

    return 0;
}
