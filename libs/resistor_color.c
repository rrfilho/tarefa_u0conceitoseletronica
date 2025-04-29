#include <stdbool.h>

#define RANGE 24
#define LOW_RANGE 0.95
#define HIGH_RANGE 1.05

static char HEX_BLACK[] = "000000";
static char HEX_BROWN[] = "964b00";
static char HEX_RED[] = "ff0000";
static char HEX_ORANGE[] = "f53000";
static char HEX_YELLOW[] = "fff112";
static char HEX_GREEN[] = "00ff00";
static char HEX_BLUE[] = "0000ff";
static char HEX_VIOLET[] = "9b539f";
static char HEX_GRAY[] = "5b5b5b";
static char HEX_WHITE[] = "ffffff";

static char BLACK[] = "Preto";
static char BROWN[] = "Marrom";
static char RED[] = "Vermelho";
static char ORANGE[] = "Laranja";
static char YELLOW[] = "Amarelo";
static char GREEN[] = "Verde";
static char BLUE[] = "Azul";
static char VIOLET[] = "Violeta";
static char GRAY[] = "Cinza";
static char WHITE[] = "Branco";

static const int ranges[RANGE] = {
    10, 11, 12, 13, 15, 16, 18, 20, 
    22, 24, 27, 30, 33, 36, 39, 43, 
    47, 51, 56, 62, 68, 75, 82, 91
};

static bool is_out_of_range(float resistance) {
    return resistance < 510 || resistance > 100000;
}

static bool is_inside_range(int multiplier, int range, float resistance) {
    return 
        resistance >= LOW_RANGE * range * multiplier && 
        resistance <= HIGH_RANGE * range * multiplier;
}

static char* band_hex_color(int digit) {
    switch (digit) {
        case 0: return HEX_BLACK;
        case 1: return HEX_BROWN;
        case 2: return HEX_RED;
        case 3: return HEX_ORANGE;
        case 4: return HEX_YELLOW;
        case 5: return HEX_GREEN;
        case 6: return HEX_BLUE;
        case 7: return HEX_VIOLET;
        case 8: return HEX_GRAY;
        case 9: return HEX_WHITE;
        default: return HEX_BLACK;
    }
}

static char* band_color(int digit) {
    switch (digit) {
        case 0: return BLACK;
        case 1: return BROWN;
        case 2: return RED;
        case 3: return ORANGE;
        case 4: return YELLOW;
        case 5: return GREEN;
        case 6: return BLUE;
        case 7: return VIOLET;
        case 8: return GRAY;
        case 9: return WHITE;
        default: return BLACK;
    }
}

static char* multiplier_hex_color(int multiplier) {
    switch (multiplier) {
        case 10: return HEX_BROWN;
        case 100: return HEX_RED;
        case 1000: return HEX_ORANGE;
        case 10000: return HEX_YELLOW;
        default: return HEX_BLACK;
    }
}

static char* multiplier_color(int multiplier) {
    switch (multiplier) {
        case 10: return BROWN;
        case 100: return RED;
        case 1000: return ORANGE;
        case 10000: return YELLOW;
        default: return BLACK;
    }
}

static int* bands_indexes(int multiplier, int range) {
    static int result[3];
    int first_digit = (int) range/10;
    int second_digit = range%10;
    result[0] = first_digit;
    result[1] = second_digit;
    result[2] = multiplier;
    return result;
}

static int* resistor_indexes(float resistance) {
    if (is_out_of_range(resistance)) return 0;
    for (int multiplier = 10; multiplier < 100000; multiplier = multiplier*10)
        for (int index = 0; index < RANGE; index++) 
            if (is_inside_range(multiplier, ranges[index], resistance)) 
                return bands_indexes(multiplier, ranges[index]);
}

char** resistor_hex_colors(float resistance) {
    static char* result[3];
    int* indexes = resistor_indexes(resistance);
    result[0] = band_hex_color(indexes[0]);
    result[1] = band_hex_color(indexes[1]);
    result[2] = multiplier_hex_color(indexes[2]);
    return result;
}

char** resistor_colors(float resistance) {
    static char* result[3];
    int* indexes = resistor_indexes(resistance);
    result[0] = band_color(indexes[0]);
    result[1] = band_color(indexes[1]);
    result[2] = multiplier_color(indexes[2]);
    return result;
}