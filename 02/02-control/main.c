#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "stdio_task/stdio_task.h" // Для обработки многосимвольных команд

int main()
{
    stdio_task_init(); // Иницилизация обрабоки команд
    stdio_init_all(); // Инициализация система вводы/вывода
    char* result;
    while(1) {
        result = stdio_task_handle();  // Получаем адрес
        
        if (result != NULL)  // Если адрес не 0 (не NULL)
        {
            printf("Команда: %s\n", result);  // %s идет по адресу и читает символы до \0
        }
    }
}