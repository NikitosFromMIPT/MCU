#include "pico/stdlib.h"
#include "stdio.h"
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
int main()
{
    stdio_init_all();
    while (1)
    {
        printf("Device name: '%s'\n", DEVICE_NAME);
        sleep_ms(1000);
    }
}