#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>

#define NUM_LEDS 4

#define STACK_SIZE 512
#define PRIORITY 5
#define DEBOUNCE_COUNT 3 // Anzahl gleiche Samples für stabil

// #define FORCE_STACK_CRASH

static int delay_ms = 500;

typedef struct
{
    int stable_state;
    int last_read;
    int count;
} button_debounce_t;

static bool evaluate_button(button_debounce_t *button_state, int new_state, const char *button_name)
{
    bool state_changed = false;

    if (button_state->last_read == new_state)
    {
        button_state->count++;
    }
    else
    {
        button_state->count = 0;
    }
    button_state->last_read = new_state;

    if (button_state->count >= DEBOUNCE_COUNT)
    {
        if (button_state->stable_state != new_state)
        {
            button_state->stable_state = new_state;
            state_changed = true;
            if (new_state == 1)
            {
                printk("%s aktiv!\n", button_name);
            }
            else
            {
                printk("%s inaktiv!\n", button_name);
            }
        }
        button_state->count = DEBOUNCE_COUNT; // verhindere Overflow
    }
    return state_changed;
}

#if defined FORCE_STACK_CRASH
#define HUGE_VARIABLE_SIZE 512
static int count_down = 10;

static void cause_crash(void)
{
    volatile uint8_t stack_eater[HUGE_VARIABLE_SIZE];

    for (int i = 0; i < HUGE_VARIABLE_SIZE; i++)
    {
        stack_eater[i] = i;
    }
}
#endif

static void button_thread(void *arg1, void *arg2, void *arg3)
{
    static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(DT_NODELABEL(button1), gpios);
    static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(DT_NODELABEL(button3), gpios);

    button_debounce_t button1_state = {0};
    button_debounce_t button3_state = {0};

    /* Buttons konfigurieren */
    if (!device_is_ready(button1.port) || !device_is_ready(button3.port))
    {
        return;
    }

    gpio_pin_configure_dt(&button1, GPIO_INPUT);
    gpio_pin_configure_dt(&button3, GPIO_INPUT);

    while (1)
    {
        /* Buttons abfragen */
        int b1 = gpio_pin_get_dt(&button1); // gedrückt (active-low)
        int b3 = gpio_pin_get_dt(&button3); // gedrückt (active-low)

        evaluate_button(&button1_state, b1, "Button1");
        evaluate_button(&button3_state, b3, "Button3");

        k_sleep(K_MSEC(10));

#if defined FORCE_STACK_CRASH
        if (--count_down == 0)
        {
            count_down = 10;
            cause_crash();
        }
#endif
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
    const struct device *leds = DEVICE_DT_GET(DT_PATH(leds));

    if (!device_is_ready(leds))
    {
        return -1;
    }

    int current = 0;
    while (1)
    {
        /* Alle LEDs ausschalten */
        for (int i = 0; i < NUM_LEDS; i++)
        {
            led_off(leds, i);
        }

        /* Aktuelle LED einschalten */
        led_on(leds, current);

        /* Nächste LED */
        current = (current + 1) % NUM_LEDS;

        aufgaben_erledigen();
        k_sleep(K_MSEC(delay_ms));
    }

    return 0;
}

K_THREAD_DEFINE(button_id, STACK_SIZE, button_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
