#pragma once
#include "esphome.h"
#include <WiFi.h>

class MJPEGServer : public esphome::Component {
 public:
  int port_{3333};
  int fps_{10};
  WiFiServer *server_{nullptr};
  unsigned long last_frame_{0};

  void set_port(int port) { port_ = port; }
  void set_fps(int fps) { fps_ = fps; }

  void setup() override {
    server_ = new WiFiServer(port_);
    server_->begin();
    ESP_LOGI("mjpeg_server", "MJPEG server iniciado na porta %d", port_);
  }

  void loop() override {
    WiFiClient client = server_->available();
    if (!client) return;

    unsigned long now = millis();
    if (now - last_frame_ < (1000 / fps_)) return;
    last_frame_ = now;

    auto img = id(espcam).take_photo();
    if (!img) return;

    uint32_t len = img->size();
    client.write((uint8_t*)&len, 4);
    client.write(img->data(), len);
  }
};
