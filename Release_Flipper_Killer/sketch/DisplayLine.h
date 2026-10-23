#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\DisplayLine.h"
#pragma once

#include <stddef.h>
#include <stdint.h>

void fitDisplayLine(char* output, size_t output_size, const char* input);
uint8_t resolveDisplayTextSize(bool small_print, uint8_t requested_size);
