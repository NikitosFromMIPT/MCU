#include "pico/stdlib.h"
#include "stdio.h"
#include "led/led.h"
#include "main.h"


int main()
{
    stdio_init_all();
    led_init();
    while (1)
    {
        char symbol = getchar();
        switch (symbol)
        {
        case 'e':
            led_enable();
            printf("led ON\n");
            break;

        case 'd':
            led_disable();
            printf("led OF\n");
            break; 

        case 'v':
            printf("Device name: '%s'\n", DEVICE_NAME);
            printf("Version: '%s'\n", DEVICE_VRSN);
            
            break; 

        default:
            printf("ERROR: \"%c\": %d", symbol, symbol);
            printf(" - not a command\n");
            break;
        }
    }
}