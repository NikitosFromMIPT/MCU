#include "led_task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint LED_PIN = 25;
uint LED_BLINK_PERIOD_US = 500000;
bool state_bool = 0;
uint64_t led_ts;
led_state_t led_state; // глобальная переменная

void led_task_init(void)
{
    led_state = LED_STATE_OFF;
    led_ts = 0;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_task_handle(void) // обрабочик led отвечает за поддержание режима
{
    switch (led_state)
    {
        case LED_STATE_OFF:
            gpio_put(LED_PIN, 0);
            break;
        case LED_STATE_ON:
            gpio_put(LED_PIN, 1);
            break;
        case LED_STATE_BLINK:
            if (time_us_64() > led_ts)
            {
                led_ts = time_us_64() + (LED_BLINK_PERIOD_US / 2);
                state_bool = !state_bool;
                gpio_put(LED_PIN, state_bool);
            }
            break;
        default:
            break;
    }
}

void led_task_state_set(led_state_t state) // отвечает за смену состаяния 
{
    led_state = state; // можно добавить код для сбросов всяких таймеров и тд и тп для более коректной работы после смены режима
}