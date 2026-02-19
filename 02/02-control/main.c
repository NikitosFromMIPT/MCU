#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "stdio_task/stdio_task.h" // Для обработки многосимвольных команд
#include "protocol_task/protocol_task.h"
#include "led_task/led_task.h"
#include "main.h"

void version_callback(const char* args) {printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);}
void led_on_callback(const char* args) 
{
    led_task_state_set(1);
    printf("LED ON\n");
}
void led_off_callback(const char* args) 
{
    led_task_state_set(0);
    printf("LED OFF\n");
}
void led_blink_callback(const char* args) 
{
    led_task_state_set(2);
    printf("LED BLINK\n");
}

api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "start working od the LED"},
    {"off", led_off_callback, "stop working od the LED"},
    {"blink", led_blink_callback, "start blinkig of the LED"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_task_init(); // Иницилизация считывания многосимвольных команд
    stdio_init_all(); // Инициализация система вводы/вывода
    protocol_task_init(device_api); // Иницилизация обработки команд 
    led_task_init(); // Иницилизация задачи светодиод
    while(1) 
    {
        protocol_task_handle(stdio_task_handle()); // считываем команду взятие строки
        led_task_handle(); // обрабатывает режим работы светодиода
    }
}