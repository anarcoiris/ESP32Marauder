#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\BeaconFrame.h"
#pragma once

#include <stddef.h>
#include <stdint.h>

bool setBeaconFrameChannel(
  uint8_t* frame,
  size_t frame_size,
  size_t ssid_length,
  uint8_t channel
);
