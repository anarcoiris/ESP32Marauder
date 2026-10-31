#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\TDongleBus.cpp"
#include "TDongleBus.h"

#include <Arduino.h>

void deselectTDongleSharedSpi(uint8_t tftCsPin, uint8_t sdCsPin) {
  // The APA102 shares the TFT/SD clock and data lines on shipped T-Dongles.
  pinMode(tftCsPin, OUTPUT);
  digitalWrite(tftCsPin, HIGH);
  pinMode(sdCsPin, OUTPUT);
  digitalWrite(sdCsPin, HIGH);
}
