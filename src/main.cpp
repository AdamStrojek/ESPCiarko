#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

#include "config.h"
#include "log.h"
#include "hood_bt.h"
#include "hob2hood_ir.h"
#include "hood.h"


Hood current;
Hood target;


const char* ssid = WIFI_SSID;
const char* password = WIFI_PSWD;


void setup() {
    Serial.begin(9600);

    Serial.println("Starting ESPCiarko Remote");


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    log_init();
    log_print("Setting up OTA");

    ArduinoOTA.setHostname("espciarko");

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            hob2hood_ir_halt();
            hood_bt_halt();

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            log_print("Start updating " + type);
        })
        .onEnd([]() {
            log_print("End");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) log_print("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) log_print("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) log_print("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) log_print("Receive Failed");
            else if (error == OTA_END_ERROR) log_print("End Failed");
        });

    ArduinoOTA.begin();

    log_print("Ready");
    log_print("IP address: " + WiFi.localIP().toString());

    hood_bt_init();

    hob2hood_ir_init();
}

void loop() {
    hood_bt_process(current);

    hob2hood_ir_process(target);

    if(target.speed != UNKNOW) {
        if(current.speed > target.speed) {
            hood_bt_send_command(HOOD_CMD_MINUS);
        } else if(current.speed < target.speed) {
            hood_bt_send_command(HOOD_CMD_PLUS);
        }
        target.speed = UNKNOW;
    }

    if(target.light != UNKNOW) {
        if( current.light != target.light) {
            hood_bt_send_command(HOOD_CMD_LIGHT);
            current.light = target.light;
        }
        target.light = UNKNOW;
    }

    ArduinoOTA.handle();
}
