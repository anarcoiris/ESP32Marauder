#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

class MobileAPClass {
public:
  void begin();
  void update();
  void startAP();
  void stopAP();
  bool isActive() const { return ap_active; }

private:
  bool ap_active = false;
  unsigned long button_press_start = 0;
  bool button_pressed = false;
  DNSServer dnsServer;
  AsyncWebServer server{80};
};

extern MobileAPClass MobileAP;
void mobileLogWrite(uint8_t c);
String getMobileLogs();
