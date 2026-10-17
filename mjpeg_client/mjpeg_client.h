#pragma once
#include "esphome.h"
#include <WiFi.h>
#include <TJpg_Decoder.h>

class MJPEGClient : public esphome::Component {
 public:
  WiFiClient client;

  void setup() override {
    TJpgDec.setJpgScale(1);
    TJpgDec.setSwapBytes(true);

    TJpgDec.setCallback([](int x, int y, int w, int h, uint16_t *bitmap){
      for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
          id(my_display).draw_pixel(x+i, y+j, bitmap[j*w+i]);
        }
      }
    });

    client.connect("IP_DO_ESP_CAM", 3333);
  }

  void loop() override {
    if (!client.connected()) {
      client.connect("IP_DO_ESP_CAM", 3333);
      return;
    }

    uint32_t len;
    if (client.readBytes((char*)&len, 4) != 4) return;

    uint8_t *buf = (uint8_t*) heap_caps_malloc(len, MALLOC_CAP_SPIRAM);
    if (!buf) return;

    client.readBytes(buf, len);

    TJpgDec.drawJpg(0, 0, buf, len);

    free(buf);
  }
};
