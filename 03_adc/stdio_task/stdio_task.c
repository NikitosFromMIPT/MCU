#include "stdio_task.h"
#include "stdio.h"
#include "pico/stdlib.h"

#define COMMAND_BUF_LEN 128

char command[COMMAND_BUF_LEN] = {0}; //ввести массив charов длинной COMMAND_BUF_LEN; = {0} - специальный синтаксис, который обнуляет весь массив
int command_buf_idx;

void stdio_task_init()
{
	command_buf_idx = 0;
}

char* stdio_task_handle()  // Возвращает адрес, где лежит строка
{
	int symbol = getchar_timeout_us(0); //функция из библиотеки Pico, которая пытается прочитать символ (сохраняет код символа)
	if (symbol == PICO_ERROR_TIMEOUT) // getchar  выдаёт код символа который ввели, если нчего не ввели - ошибку  
	{
		return NULL;
	}

	putchar(symbol); // Возращает символ в терминал; Это нужно чтобы пользователь видел что он печатает

	if (symbol == '\r' || symbol == '\n') // \r - возврат каретки, \n - новая строка, короче нажатие enter
	{
		command[command_buf_idx] = '\0'; // Добавляем нуль-терминатор в конец строки, он ограничет строку
		command_buf_idx = 0;
		
		//printf("received string: '%s'\n", command); // закомитить чтобы не выводило дважды строку

	return command;
	}

	if (command_buf_idx >= COMMAND_BUF_LEN - 1)
	{
		command_buf_idx = 0;
		return NULL;
	}

	command[command_buf_idx] = symbol;
	command_buf_idx++;
	return NULL;
}