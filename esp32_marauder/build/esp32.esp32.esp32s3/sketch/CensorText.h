#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\CensorText.h"
#pragma once

#include <stddef.h>

namespace CensorText {
bool looksLikeMac(const char* value);
void name(char* value);
void mac(char* value);
void identifier(char* value);
}
