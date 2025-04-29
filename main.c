#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "libs/bootsel.h"
#include "libs/display.h"
#include "libs/adc.h"
#include "libs/led_matrix.h"
#include "libs/resistor_color.h"

#define KNOWN_RESISTOR 10000.0
#define ADC_RESOLUTION 4095.0

void init() {
    stdio_init_all();
    bootsel_init();
    display_init();
    adc_mean_init();
    led_matrix_init();
}

float calculate_resistance(float value) {
    return (KNOWN_RESISTOR * value) / (ADC_RESOLUTION - value);
}

int main() {
    init();
    while (true) {
        float mean = adc_mean_read();
        float resistance = calculate_resistance(mean);
        char** colors = resistor_colors(resistance);
        printf("%s, %s, %s\n", colors[0], colors[1], colors[2]);
        display_update(mean, resistance, colors);
        char** hex_colors = resistor_hex_colors(resistance);
        led_matrix_draw(hex_colors[0], hex_colors[1], hex_colors[2]);
        sleep_ms(700);
    }
}


/*
    Equação para encontrar a resistência desconhecida, utilizando o método do divisor de tensão
    Sabendo que a tensão fornecida pelo Raspberry pi pico é de 3.3V e os resistores conhecido e 
    desconhecido estão em série, a corrente que percorre o circuito é definida por:
    
    I = 3.3/ (R_conhecido + R_desconhecido)

    Visto que o valor de corrente é igual para os resistores conhecido e desconhecido,
    a tensão sobre a resistência desconhecida é dada por:

    V = R_desconhecido * I => V = R_desconhecido * 3.3/ (R_desconhecido + R_conhecido)

    Após a conversão analógico-digital, sabemos que a tensão digital sobre o resistor desconhecido
    é expressa por:

    Vd = 3.3 * ADC_lido/ADC_resolucao

    Uma vez que Vd deve ser igual a V (Vd = V), temos:

    3.3 * ADC_lido/ADC_resolucao = 3.3 * R_desconhecido /(R_desconhecido + R_conhecido)

    resolvendo a equação para R_desconhecido, temos:

    R_desconhecido * ADC_resolucao = ADC_lido * (R_desconhecido + R_conhecido)
    R_desconhecido * ADC_resolucao = ADC_lido * R_desconhecido + ADC_lido * R_conhecido

    R_desconhecido * ADC_resolucao - ADC_lido * R_desconhecido = ADC_lido * R_conhecido
    R_desconhecido * (ADC_resolucao - ADC_lido) = ADC_lido * R_conhecido
    R_desconhecido = ADC_lido * R_conhecido / (ADC_resolucao - ADC_lido)
*/ 