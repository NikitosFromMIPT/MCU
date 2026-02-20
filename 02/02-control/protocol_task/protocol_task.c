#include "protocol_task.h"
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "string.h"

//переменные для хранения указателя на массив команд и его размер:
static api_t* api = {0};
static int commands_count = 0;

void protocol_task_init(api_t* device_api)
{
    api = device_api; //указатель на массив команд
    while (device_api[commands_count].command_name != NULL) 
    {
        commands_count++;
    }
    printf(" Найденно '%d' команд\n", commands_count);    
}

void help_handle(const char* args)
{

    printf("\nAvailable commands:\n");
    printf("--------------------\n");
    
    for (int i = 0; api[i].command_name != NULL; i++) {
        // Используем поле command_help для вывода описания
        printf("  '%s' - '%s'\n", api[i].command_name, api[i].command_help);
    }
    printf("--------------------\n");
}
void protocol_task_handle(char* command_string)
{
    if (!command_string) //	Добавляем в обработчик проверку на то, что `command_string` не равно `NULL`.
    {
        return;
    }

// логика обработки полученной строки. Делим ее на команду и аргументы:
    const char* command_name = command_string;
    const char* command_args = NULL;

    char* space_symbol = strchr(command_string, ' ');

    if (space_symbol)
    {  
        *space_symbol = '\0';
        command_args = space_symbol + 1;
    }
    else
    {
        command_args = "";
    }

  
    // Добавляем вывод найденных имени команды и ее аргументов;
    printf("Получена команда: '%s'\n", command_name);
    printf("Аргументы: '%s'\n", command_args);
  
    // в цикле проходим по массиву команд `api` и ищем совпадение имени команды;

    for (int i = 0; i < commands_count; i++)
    {
        if (strcmp(api[i].command_name, command_name) == 0) // определяем совпадает ли команда  с именем команды в массиве `api`
        {
            api[i].command_callback(command_args);
            return;
        }
    }
    
    // выводим ошибку, если команда не была найдена в списке команд
    printf("Ошибка: неизвестная команда '%s'\n", command_name);
    return;
}