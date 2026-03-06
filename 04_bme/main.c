#include "main.h"
#include "pico/stdlib.h" // Базовые функции работы с микроконтролером
#include "stdio.h"  // Для работы с вводом/выводом данных standart input output
#include "stdio_task/stdio_task.h" // Для обработки многосимвольных команд
#include "led_task/led_task.h"
#include "protocol_task.h"
#include "bme280_driver.h"
#include "hardware/i2c.h"


uint32_t read(uint32_t addr) // принимает адресс превращает его в указатель и возвращает прочитанный
{
    return *(volatile uint32_t*)addr; 
}


void rp2040_i2c_read(uint8_t* buffer, uint16_t length) // чтение из I2C шины
{
    /*
                        Как работает i2c_read_timeout_us:
    1 Аргумент: Экземпляр I2C в RP2040: 
            i2c0 - использует пины: (0,1), (4,5), (8,9), (12,13), (16,17), (20,21)
            i2c1 - использует пины: (2,3), (6,7), (10,11), (14,15), (18,19), (26,27)
    2 Аргумент: 7-битный адрес устройства:
            0x76 ести SDO подтянуто к GND (на готовых устройствах почти всегда)
            0x77 ести SDO подтянуто к Vddio
    3 Аргумент: Указатель на буфер для приема данных
    4 Аргумент: Количество байт для чтения
    5 Аргумент: Не отправлять STOP после транзакции
    6 Аргумент: Таймаут в микросекундах
    */
	i2c_read_timeout_us(i2c1, 0x76, buffer, length, false, 100000);
    // формирует транзакцию: START → [addr + R] → чтение байт → ... → STOP
}
void rp2040_i2c_write(uint8_t* data, uint16_t size) // запись в I2C шину
{
    /*
    3 Аргумент: Буфер с данными для отправки
    4 Аргумент: Количество байт для отправки
    */
	i2c_write_timeout_us(i2c1, 0x76, data, size, false, 100000);
    //формирует транзакцию: START → [addr + W] → data[0] → data[1] → ... → data[len-1] → (STOP если !nostop)
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
void read_reg__callback(const char* args)
{
    unsigned int addr, N;  // Используем unsigned int для sscanf

    
    if (sscanf(args, "%x %x", &addr, &N) != 2)
    {  
        printf("Usage: read_reg <hex_addr> <hex_N>\n");
        return;
    } 

    if (addr > 0xFF)
    {
        printf("Error: addr must be <= 0xFF\n");
        return;
    }
    
    if (N > 0xFF)
    {
        printf("Error: N must be <= 0xFF\n");
        return;
    }
    
    if (addr + N > 0x100)
    {
        printf("Error: addr + N must be <= 0x100\n");
        return;
    }


    uint8_t buffer[256] = {0}; // массив для сохранения регистров
    
    // Приводим к uint8_t при передаче
    bme280_read_regs((uint8_t)addr, buffer, (uint8_t)N);
    
    for (int i = 0; i < (int)N; i++)
    {
        printf("bme280 register [0x%X] = 0x%X\n", addr + i, buffer[i]);
    }
}
void write_reg_callback(const char* args)
{
    unsigned int addr, value;
    
    if (sscanf(args, "%x %x", &addr, &value) != 2)
    {
        printf("Usage: write_reg <hex_addr> <hex_value>\n");
        return;
    }
    
    // Проверки
    if (addr > 0xFF)
    {
        printf("Error: register address must be <= 0xFF\n");
        return;
    }
    
    if (value > 0xFF)
    {
        printf("Error: value must be <= 0xFF\n");
        return;
    }
    
    // Запись в регистр
    bme280_write_reg((uint8_t)addr, (uint8_t)value);
    printf("Written 0x%02X to register 0x%02X\n", value, addr);
}
void bme280_read_temp_raw_callback(const char* args) 
{
    uint16_t value = bme280_read_temp_raw();
    printf("Temperature in counts: %u\n", value);
}
void bme280_read_pres_raw_callback(const char* args) 
{
    uint16_t value = bme280_read_pres_raw();
    printf("Presure in counts: %u\n", value);
}
void bme280_read_hum_raw_callback(const char* args) 
{
    uint16_t value = bme280_read_hum_raw();
    printf("Humidity in counts: %u\n", value);
}
void bme280_read_temp_C_callback(const char* args) 
{
    float value = bmp280_get_temperature_celsius();
    printf("Temperature: %f °C\n", value);
}
void bme280_read_pres_Pa_callback(const char* args) 
{
    float value = bmp280_get_pressure_hpa();
    printf("Presure: %f hPa\n", value);
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
    {"read_reg", read_reg__callback, "read registr, put adrees and count of registers needable to read (on hex)"},
    {"write_reg", write_reg_callback, "write registr, put adrees and value (on hex)"},
    {"temp_raw", bme280_read_temp_raw_callback, "Print value of temerature in counts"},
    {"pres_raw", bme280_read_pres_raw_callback, "Print value of presure in counts"},
    {"hum_raw", bme280_read_hum_raw_callback, "Print value of humidity in counts (Only for BME 280)"},
    {"temp", bme280_read_temp_C_callback, "Print value of temerature in celsius"},
    {"press", bme280_read_pres_Pa_callback, "Print value of presure in hecto Paskals"},
    {"help", help_callback, "print discribtion of commands"},
    {NULL, NULL, NULL},
};


int main()
{
    stdio_task_init(); // Иницилизация считывания многосимвольных команд
    stdio_init_all(); // Инициализация система вводы/вывода
    protocol_task_init(device_api); // Иницилизация обработки команд 
    led_task_init(); // Иницилизация задачи светодиод
    i2c_init(i2c1, 100000); // Инициализирует модуль I2C с заданной скоростью (в Гц)
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    bme280_init(rp2040_i2c_read, rp2040_i2c_write);
    while(1) 
    {
        protocol_task_handle(stdio_task_handle()); // считываем команду взятие строки
        led_task_handle(); // обрабатывает режим работы светодиода
    }
}