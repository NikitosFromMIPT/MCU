#include "adc_task.h"
#include "hardware/adc.h"
#include "stdio.h"
#include "pico/stdlib.h"

#define ADC_GPIO_PIN     26
#define ADC_CHANNEL     0
#define ADC_REF_VOLTAGE 3.3f
#define ADC_RESOLUTION  4095.0f

void adc_task_init(void)
{
    adc_init();
    adc_gpio_init(ADC_GPIO_PIN);
}


float adc_task_measure(void)
{
    adc_select_input(ADC_CHANNEL);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (voltage_counts / ADC_RESOLUTION) * ADC_REF_VOLTAGE;
    return voltage_V;
}