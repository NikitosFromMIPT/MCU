#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "stdio_task/stdio_task.h" // Для обработки многосимвольных команд
#include "protocol_task/protocol_task.h"
#include "main.h"

char* result;
void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_task_init(); // Иницилизация обрабоки команд
    stdio_init_all(); // Инициализация система вводы/вывода
    protocol_task_init(device_api);
    while(1) 
    {
        result = stdio_task_handle();  // Получаем адрес
        
        if (result != NULL)  // Если адрес не 0 (не NULL)
        {
            printf("Команда: %s\n", result);  // %s идет по адресу и читает символы до \0
            protocol_task_handle(result);
        }
    }
}