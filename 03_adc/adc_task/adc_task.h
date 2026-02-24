#pragma once

typedef enum
{
	ADC_TASK_STATE_IDLE = 0,
	ADC_TASK_STATE_RUN = 1,
} adc_task_state_t;

void adc_task_init(void);
float adc_task_measure(void);
float adc_task_read_temperature(void);

void adc_task_handle(void); // функция обработчика, отыечает за реализацию состояний
void adc_task_state_set(adc_task_state_t state);// функция для смены состояния