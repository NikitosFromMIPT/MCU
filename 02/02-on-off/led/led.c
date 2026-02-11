#include "led.h"
#include "hardware/gpio.h"
const uint LED_PIN = 25;

void led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_enable(void) {
     gpio_put(LED_PIN, 1);
}

void led_disable(void) {
     gpio_put(LED_PIN, 0);
}