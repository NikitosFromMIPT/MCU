#include "adc_task.h"
#include "hardware/adc.h"
#include "stdio.h"
#include "pico/stdlib.h"

// #define - то что нельзя менять во время исполнения
#define ADC_GPIO_PIN     26
#define ADC_TEMP_CHANNEL     4
#define ADC_CHANNEL     0
#define ADC_REF_VOLTAGE 3.3f
#define ADC_RESOLUTION  4095.0f

adc_task_state_t adc_state;
uint64_t adc_ts;
uint ADC_TASK_MEAS_PERIOD_US =  1000000; // микросекунды 10^-6 c

void adc_task_init(void)
{
    adc_init();
    adc_gpio_init(ADC_GPIO_PIN);
    adc_set_temp_sensor_enabled(true);
    adc_ts = 0;
    adc_state = ADC_TASK_STATE_IDLE;
}


float adc_task_measure(void)
{
    adc_select_input(ADC_CHANNEL);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (voltage_counts / ADC_RESOLUTION) * ADC_REF_VOLTAGE;
    return voltage_V;
}

float adc_task_read_temperature(void)
{
    adc_select_input(ADC_TEMP_CHANNEL);
    uint16_t temp_counts = adc_read();
    float temp_V = (temp_counts / ADC_RESOLUTION) * ADC_REF_VOLTAGE;
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
    return temp_C;
}

void adc_task_handle(void)
{
    // конструкцйия switch(объект) работает так: в case условие, если объект = условие выполнить
 switch (adc_state)
    {
        case ADC_TASK_STATE_IDLE:
            break;

        case ADC_TASK_STATE_RUN:
            if (time_us_64() > adc_ts)
            {
                adc_ts = time_us_64() + ADC_TASK_MEAS_PERIOD_US;

                float v = adc_task_measure();
                float t = adc_task_read_temperature();
                printf("%f %f\n", v, t);                    
            }
            break;

        default:
            break;
    }
}

void adc_task_state_set(adc_task_state_t state)
{
    adc_state = state;
}