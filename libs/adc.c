#include "adc.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define ADC_PIN 28
#define ADC_SELECTOR 2
#define SAMPLES 500.0

void adc_mean_init() {
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_SELECTOR);
}

float adc_mean_read() {
    float sum = 0.0f;
    for (int i = 0; i < SAMPLES; i++) {
      sum += adc_read();
      sleep_ms(1);
    }
    return sum / SAMPLES;
}