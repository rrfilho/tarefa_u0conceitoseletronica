#include "led_matrix.h"
#include <math.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.h"
#include <stdio.h>

#define PIXELS 25
#define COLOR_STRING_SIZE 6
#define FIRST_BYTE 24
#define SECOND_BYTE 16
#define THIRD_BYTE 8
#define LED_MATRIX_PIN 7
#define OFF 0
#define BRIGHTNESS_FACTOR 0.2

static const uint MAP[PIXELS] = {
    24,23,22,21,20,
    15,16,17,18,19,
    14,13,12,11,10,
     5, 6, 7, 8, 9,
     4, 3, 2, 1, 0
};
static const uint BANDS[PIXELS] = {
    1, 0, 2, 0, 3,
    1, 0, 2, 0, 3,
    1, 0, 2, 0, 3,
    1, 0, 2, 0, 3,
    1, 0, 2, 0, 3
};
static const PIO PIO_ID = pio0;
static uint sm;

void led_matrix_init() {
    set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(PIO_ID, &pio_matrix_program);
    sm = pio_claim_unused_sm(PIO_ID, true);
    pio_matrix_program_init(PIO_ID, sm, offset, LED_MATRIX_PIN);
}

static uint to_decimal(char hex) {
    if (hex >= '0' && hex <= '9') return hex - '0';
    if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
    if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
    return 0;
}

static uint to_rgb(const char hex[COLOR_STRING_SIZE]) {
    uint r = (((to_decimal(hex[0]) * 16) + to_decimal(hex[1]))) * BRIGHTNESS_FACTOR;
    uint g = (((to_decimal(hex[2]) * 16) + to_decimal(hex[3]))) * BRIGHTNESS_FACTOR;
    uint b = (((to_decimal(hex[4]) * 16) + to_decimal(hex[5]))) * BRIGHTNESS_FACTOR;
    return (g << FIRST_BYTE) | (r << SECOND_BYTE) | (b << THIRD_BYTE);
}

void led_matrix_draw(char first_band[COLOR_STRING_SIZE], char second_band[COLOR_STRING_SIZE], char third_band[COLOR_STRING_SIZE]) {
    uint colors[4] = { OFF, to_rgb(first_band), to_rgb(second_band), to_rgb(third_band) };
    for (uint i = 0; i < PIXELS; i++) {
        uint index = MAP[i];
        uint led_value = BANDS[index];
        pio_sm_put_blocking(PIO_ID, sm, colors[led_value]);
    }
}