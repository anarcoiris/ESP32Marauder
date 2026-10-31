#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\DualSerial.h"
#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <HWCDC.h>

extern void mobileLogWrite(uint8_t c);

class DualSerialClass : public Stream {
public:
  void begin(unsigned long baud = 115200) {
    HWCDCSerial.begin(baud);
    HWCDCSerial.setTxTimeoutMs(10);
    Serial0.begin(baud, SERIAL_8N1, 44, 43); // RX=44, TX=43
  }

  int available() override {
    int a = HWCDCSerial.available();
    if (a > 0) return a;
    return Serial0.available();
  }

  int read() override {
    if (HWCDCSerial.available() > 0) return HWCDCSerial.read();
    return Serial0.read();
  }

  int peek() override {
    if (HWCDCSerial.available() > 0) return HWCDCSerial.peek();
    return Serial0.peek();
  }

  void flush() override {
    HWCDCSerial.flush();
    Serial0.flush();
  }

  size_t write(uint8_t c) override {
    Serial0.write(c);
    size_t res = HWCDCSerial.write(c);
    HWCDCSerial.flush();
    mobileLogWrite(c);
    return res;
  }

  size_t write(const uint8_t *buffer, size_t size) override {
    Serial0.write(buffer, size);
    size_t res = HWCDCSerial.write(buffer, size);
    HWCDCSerial.flush();
    for (size_t i = 0; i < size; i++) {
      mobileLogWrite(buffer[i]);
    }
    return res;
  }

  operator bool() const {
    return true;
  }
};

extern DualSerialClass DualSerial;
