#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
int main()
{
    stdio_init_all(); // Инициализация система вводы/вывода
    while (1)
    {
        char symbol = getchar(); //считывание введенного символа из PUTTY терминала:
        printf("ASCII код: %d\n", symbol);
    }
}