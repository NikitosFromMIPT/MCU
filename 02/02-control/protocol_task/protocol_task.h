#pragma once
// typedef - создаём новый тип данных;
typedef void(*command_callback_t)(const char* args); //можно объявлять переменные типа "указатель на функцию"
// функция принмает строку с аргументми и ничего не выводит.
typedef struct
{
	const char* command_name; // Имя команды
	command_callback_t command_callback; // Указатель на функцию-обработчик
	const char* command_help;// Текст помощи
} api_t;

void protocol_task_init(api_t* device_api); //Функция инициализации протокола.

void protocol_task_handle(char* command_string); //Функция обработки полученной команды.