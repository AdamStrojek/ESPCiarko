#ifndef CONFIG_H
#define CONFIG_H

#include <driver/gpio.h>

// Raname this file to config.h before compiling
// Insert here proper BT address to your Ciarko hood
#define HOOD_BT_ADDRESS {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
// This is default PIN for Ciarko hood
#define HOOD_BT_PIN "1234"

#define PIN_IR_RECEIVER GPIO_NUM_22

#define WIFI_SSID "My WiFi"
#define WIFI_PSWD "12345678"

#define LOG_SINK_HOST "MacBook-Pro"
#define LOG_SINK_PORT 12345

#endif