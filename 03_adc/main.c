#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "stdio_task/stdio_task.h" // Для обработки многосимвольных команд
#include "protocol_task/protocol_task.h"
#include "led_task/led_task.h"
#include "adc_task/adc_task.h"
#include "main.h"

uint32_t read(uint32_t addr)
{
    return *(volatile uint32_t*)addr; // принимает адресс превращает его в указатель и возвращает прочитанный
}
void version_callback(const char* args) {printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);}
void led_on_callback(const char* args) 
{
    led_task_state_set(LED_STATE_ON);
    printf("LED ON\n");
}
void led_off_callback(const char* args) 
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED OFF\n");
}
void led_blink_callback(const char* args) 
{
    led_task_state_set(LED_STATE_BLINK);
    printf("LED BLINK\n");
}
void led_blink_set_period_ms_callback(const char* args) 
{
    uint32_t period_ms = 0;
    sscanf(args, "%u", &period_ms); // переписывает args по протоколу %u в period_ms
    if (period_ms == 0) 
    {
        printf("error, period mast be > 0 \n");
        return;
    }
    led_task_set_blink_period_ms(period_ms);
    printf("period has been changed \n");
}
void help_callback(const char* args)
{
    printf("\nAvailable commands:\n");
    printf("--------------------\n");
    help_handle();
    printf("--------------------\n");
}




void mem_callback(const char* args)
{
    uint32_t addr;

    // Читаем адрес в шестнадцатеричном формате
    if (sscanf(args, "%x", &addr) == 1) //==1 проверка что считан 1 элемент
    {  
        // Читаем значение по адресу
        printf("addr: 0x%08x -> value: 0x%08x (%u)\n", addr, read(addr), read(addr));
    } 
    else 
    {
        printf("Usage: mem <hex_address>\n");
        printf("Example: mem 20001000\n");
        return;
    }
}

void wmem_callback(const char* args)
{
    uint32_t addr, value;
    if (sscanf(args, "%x %x", &addr, &value) == 2) //==2 проверка что считано 2 элемент
    {  
        *(volatile uint32_t*)addr = value;
        printf(" in addr: 0x%08x -> new value: 0x%08x (%u)\n", addr, value, value);
    } 
    else 
    {
        printf("error\n");
        return;
    }
}

void adc_mesure_callback(const char* args)
{
    float voltage_V;
    voltage_V = adc_task_measure();
    printf("%f\n", voltage_V);
}

api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "start working od the LED"},
    {"off", led_off_callback, "stop working od the LED"},
    {"blink", led_blink_callback, "start blinkig of the LED"},
    {"set_period", led_blink_set_period_ms_callback, "change period of LED blinking"},
    {"mem", mem_callback, "reading ptr adress"},
    {"wmem", wmem_callback, "Writes a new value to the specified address wmem(adress, value)"},
    {"get_adc", adc_mesure_callback, "print discribtion of commands"},
    {"help", help_callback, "print discribtion of commands"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_task_init(); // Иницилизация считывания многосимвольных команд
    stdio_init_all(); // Инициализация система вводы/вывода
    protocol_task_init(device_api); // Иницилизация обработки команд 
    led_task_init(); // Иницилизация задачи светодиод
    adc_task_init(); // Иницилизация задачи считывания напряжения
    while(1) 
    {
        protocol_task_handle(stdio_task_handle()); // считываем команду взятие строки
        led_task_handle(); // обрабатывает режим работы светодиода
    }
}